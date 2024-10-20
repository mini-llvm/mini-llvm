#include "mini-llvm/opt/ir/passes/Mem2Reg.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

// Ron Cytron, Jeanne Ferrante, Barry K. Rosen, Mark N. Wegman, and F. Kenneth Zadeck.
// 1991. Efficiently computing static single assignment form and the control dependence
// graph. ACM Trans. Program. Lang. Syst. 13, 4 (Oct. 1991), 451-490.
// https://doi.org/10.1145/115372.115320

namespace {

bool isPromotable(const Alloca &v) {
    return std::ranges::all_of(uses(v), [&v](const UseBase &use) {
        if (auto *store = dynamic_cast<const Store *>(use.user())) {
            return &*store->ptr() == &v && *store->value()->type() == *v.allocatedType();
        }
        if (auto *load = dynamic_cast<const Load *>(use.user())) {
            return &*load->ptr() == &v && *load->type() == *v.allocatedType();
        }
        return false;
    });
}

class Rename {
public:
    Rename(const DominatorTreeNode *root,
           const std::unordered_set<const Alloca *> &vars,
           const std::unordered_map<const Phi *, const Alloca *> &phis)
        : root_(root), vars_(vars), phis_(phis) {}

    void operator()() {
        for (const Alloca *v : vars_) {
            defs_[v].push(nullptr);
        }
        dfs(root_);
    }

private:
    const DominatorTreeNode *root_;
    const std::unordered_set<const Alloca *> &vars_;
    const std::unordered_map<const Phi *, const Alloca *> &phis_;
    std::unordered_map<const Alloca *, std::stack<const Value *>> defs_;

    void dfs(const DominatorTreeNode *node) {
        for (const Alloca *v : vars_) {
            defs_[v].push(defs_[v].top());
        }
        for (const Instruction &I : *node->block) {
            if (auto *phi = dynamic_cast<const Phi *>(&I)) {
                if (phis_.contains(phi)) {
                    const Alloca *v = phis_.at(phi);
                    defs_[v].top() = phi;
                }
            }
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*store->ptr())) {
                    if (vars_.contains(v)) {
                        defs_[v].top() = &*store->value();
                    }
                }
            }
            if (auto *load = dynamic_cast<const Load *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*load->ptr())) {
                    if (vars_.contains(v)) {
                        const Value *value = defs_[v].top();
                        assert(value != nullptr);
                        replaceAllUsesWith(*load, share(*const_cast<Value *>(value)));
                    }
                }
            }
        }
        for (BasicBlock *succ : successors(*node->block)) {
            for (Instruction &I : *succ) {
                if (auto *phi = dynamic_cast<Phi *>(&I)) {
                    if (phis_.contains(phi)) {
                        const Alloca *v = phis_.at(phi);
                        const Value *value = defs_[v].top();
                        if (value != nullptr) {
                            phi->putIncoming(*node->block, share(*const_cast<Value *>(value)));
                        } else {
                            phi->putIncoming(*node->block, std::make_shared<PoisonValue>(v->allocatedType()));
                        }
                    }
                }
            }
        }
        for (const DominatorTreeNode *child : node->children) {
            dfs(child);
        }
        for (const Alloca *v : vars_) {
            defs_[v].pop();
        }
    }
};

} // namespace

bool Mem2Reg::runOnFunction(Function &F) {
    std::unordered_set<const Alloca *> vars;
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (auto *v = dynamic_cast<const Alloca *>(&I)) {
                if (isPromotable(*v)) {
                    vars.insert(v);
                }
            }
        }
    }

    if (vars.empty()) return false;

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    std::unordered_map<const BasicBlock *, std::vector<const BasicBlock *>> DF;
    for (const BasicBlock &B : F) {
        DF[&B] = {};
    }
    for (const BasicBlock &X : F) {
        for (const BasicBlock *Y : successors(X)) {
            const BasicBlock *Z = &X;
            while (!(domTree.dominates(*Z, *Y) && Z != Y)) {
                DF[Z].push_back(Y);
                Z = domTree.node(*Z)->parent->block;
            }
        }
    }

    std::unordered_map<const Phi *, const Alloca *> phis;

    for (const Alloca *v : vars) {
        std::unordered_set<const BasicBlock *> S;
        std::queue<const BasicBlock *> Q;
        for (const UseBase &use : uses(*v)) {
            if (auto *store = dynamic_cast<const Store *>(use.user())) {
                if (&*use == &*store->ptr()) {
                    const BasicBlock *X = store->parent();
                    for (const BasicBlock *Y : DF[X]) {
                        if (!S.contains(Y)) {
                            S.insert(Y);
                            Q.push(Y);
                        }
                    }
                }
            }
        }
        while (!Q.empty()) {
            const BasicBlock *X = Q.front();
            Q.pop();
            for (const BasicBlock *Y : DF[X]) {
                if (!S.contains(Y)) {
                    S.insert(Y);
                    Q.push(Y);
                }
            }
        }
        for (const BasicBlock *B : S) {
            const Phi &phi = const_cast<BasicBlock *>(B)->prepend(std::make_shared<Phi>(v->allocatedType()));
            phis[&phi] = v;
        }
    }

    Rename(domTree.node(F.entry()), vars, phis)();

    std::vector<const Instruction *> remove(vars.begin(), vars.end());
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*store->ptr())) {
                    if (vars.contains(v)) {
                        remove.push_back(store);
                    }
                }
            }
            if (auto *load = dynamic_cast<const Load *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*load->ptr())) {
                    if (vars.contains(v)) {
                        remove.push_back(load);
                    }
                }
            }
        }
    }
    for (const Instruction *I : remove) {
        removeFromParent(*I);
    }

    return true;
}
