// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/Mem2Reg.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <queue>
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
#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

// Ron Cytron, Jeanne Ferrante, Barry K. Rosen, Mark N. Wegman, and F. Kenneth Zadeck. 1991.
// Efficiently computing static single assignment form and the control dependence graph.
// ACM Trans. Program. Lang. Syst. 13, 4 (Oct. 1991), 451-490.
// https://doi.org/10.1145/115372.115320

namespace {

bool isPromotable(const Alloca &v) {
    return std::ranges::all_of(uses(v), [&v](const UseBase &use) {
        if (auto *store = dynamic_cast<const Store *>(use.user())) {
            return &*store->ptr() == &v && &*store->value() != &v && *store->value()->type() == *v.allocatedType();
        }
        if (auto *load = dynamic_cast<const Load *>(use.user())) {
            return &*load->ptr() == &v && *load->type() == *v.allocatedType();
        }
        return false;
    });
}

class Rename {
public:
    Rename(const DTNode *root,
           const std::unordered_set<const Alloca *> &vars,
           const HashMap<const Phi *, const Alloca *> &phis)
        : root_(root), vars_(vars), phis_(phis) {}

    void operator()() {
        for (const Alloca *v : vars_) {
            defs_.put(v, nullptr);
        }
        dfs(root_);
    }

private:
    const DTNode *root_;
    const std::unordered_set<const Alloca *> &vars_;
    const HashMap<const Phi *, const Alloca *> &phis_;
    HashMap<const Alloca *, const Value *> defs_;

    void dfs(const DTNode *node) {
        HashMap<const Alloca *, const Value *> oldDefs = defs_;
        for (const Instruction &I : *node->block) {
            if (auto *phi = dynamic_cast<const Phi *>(&I)) {
                if (auto i = phis_.find(phi); i != phis_.end()) {
                    const Alloca *v = i->second;
                    defs_.put(v, phi);
                }
            }
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*store->ptr())) {
                    if (vars_.contains(v)) {
                        defs_.put(v, &*store->value());
                    }
                }
            }
            if (auto *load = dynamic_cast<const Load *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*load->ptr())) {
                    if (vars_.contains(v)) {
                        const Value *value = defs_[v];
                        if (value != nullptr) {
                            replaceAllUsesWith(*load, share(*const_cast<Value *>(value)));
                        } else {
                            replaceAllUsesWith(*load, std::make_shared<PoisonValue>(v->allocatedType()));
                        }
                    }
                }
            }
        }
        for (BasicBlock *succ : successors(*node->block)) {
            for (Instruction &I : *succ) {
                if (auto *phi = dynamic_cast<Phi *>(&I)) {
                    if (auto i = phis_.find(phi); i != phis_.end()) {
                        const Alloca *v = i->second;
                        const Value *value = defs_[v];
                        if (value != nullptr) {
                            phi->addIncoming(weaken(*const_cast<BasicBlock *>(node->block)), share(*const_cast<Value *>(value)));
                        } else {
                            phi->addIncoming(weaken(*const_cast<BasicBlock *>(node->block)), std::make_shared<PoisonValue>(v->allocatedType()));
                        }
                    }
                }
            }
        }
        for (const DTNode *child : node->children) {
            dfs(child);
        }
        defs_ = oldDefs;
    }
};

} // namespace

bool Mem2Reg::runOnFunction(Function &F) {
    bool changed = UnreachableBlockElimination().runOnFunction(F);

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

    if (vars.empty()) {
        return changed;
    }

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    HashMap<const BasicBlock *, std::vector<const BasicBlock *>> DF;
    for (const BasicBlock &B : F) {
        DF.put(&B, {});
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

    HashMap<const Phi *, const Alloca *> phis;

    for (const Alloca *v : vars) {
        std::unordered_set<const BasicBlock *> visited;
        std::queue<const BasicBlock *> Q;
        for (const UseBase &use : uses(*v)) {
            if (auto *store = dynamic_cast<const Store *>(use.user())) {
                if (&*use == &*store->ptr()) {
                    const BasicBlock *X = store->parent();
                    for (const BasicBlock *Y : DF[X]) {
                        if (visited.insert(Y).second) {
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
                if (visited.insert(Y).second) {
                    Q.push(Y);
                }
            }
        }
        for (const BasicBlock *B : visited) {
            const Phi &phi = const_cast<BasicBlock *>(B)->prepend(std::make_shared<Phi>(v->allocatedType()));
            phis.put(&phi, v);
        }
    }

    Rename(domTree.node(F.entry()), vars, phis)();

    for (const BasicBlock &B : F) {
        for (auto i = B.begin(); i != B.end();) {
            const Instruction &I = *i++;
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*store->ptr())) {
                    if (vars.contains(v)) {
                        removeFromParent(*store);
                        continue;
                    }
                }
            }
            if (auto *load = dynamic_cast<const Load *>(&I)) {
                if (auto *v = dynamic_cast<const Alloca *>(&*load->ptr())) {
                    if (vars.contains(v)) {
                        removeFromParent(*load);
                        continue;
                    }
                }
            }
        }
    }
    for (const Alloca *v : vars) {
        removeFromParent(*v);
    }

    assert(F.isWellFormed());
    return true;
}
