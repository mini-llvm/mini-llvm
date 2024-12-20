#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

#include <iterator>
#include <memory>
#include <queue>
#include <unordered_set>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingOperator.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

using namespace mini_llvm::ir;

void VerificationAnalysis::runOnFunction(const Function &F) {
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            for (const UseBase *op : I.operands()) {
                if (op->expired()) {
                    ok_ = false;
                    return;
                }
            }
        }
    }

    for (const BasicBlock &B : F) {
        if (B.empty()) {
            ok_ = false;
            return;
        }
        if (!dynamic_cast<const Terminator *>(&B.back())) {
            ok_ = false;
            return;
        }
        for (auto i = B.begin(), e = std::prev(B.end()); i != e; ++i) {
            if (dynamic_cast<const Terminator *>(&*i)) {
                ok_ = false;
                return;
            }
        }
    }

    for (const BasicBlock &B : F) {
        if (auto *ret = dynamic_cast<const Ret *>(&B.back())) {
            if (*ret->value()->type() != *F.functionType()->returnType()) {
                ok_ = false;
                return;
            }
        }
    }

    if (!hasNPredecessors(F.entry(), 0)) {
        ok_ = false;
        return;
    }

    for (const BasicBlock &B : F) {
        std::unordered_set<const BasicBlock *> preds;
        for (const BasicBlock *pred : predecessors(B)) {
            preds.insert(pred);
        }
        for (const Instruction &I : B) {
            if (auto *phi = dynamic_cast<const Phi *>(&I)) {
                if (incomingBlocks(*phi) != predecessors(B)) {
                    ok_ = false;
                    return;
                }
            }
        }
    }

    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (auto *op = dynamic_cast<const BinaryIntegerOperator *>(&I)) {
                if (*op->lhs()->type() != *op->rhs()->type()) {
                    ok_ = false;
                    return;
                }
            }
            if (auto *op = dynamic_cast<const BinaryFloatingOperator *>(&I)) {
                if (*op->lhs()->type() != *op->rhs()->type()) {
                    ok_ = false;
                    return;
                }
            }
            if (auto *select = dynamic_cast<const Select *>(&I)) {
                if (*select->trueValue()->type() != *select->falseValue()->type()) {
                    ok_ = false;
                    return;
                }
            }
        }
    }

    std::unordered_set<const BasicBlock *> S;
    std::queue<const BasicBlock *> Q;
    S.insert(&F.entry());
    Q.push(&F.entry());
    while (!Q.empty()) {
        const BasicBlock *u = Q.front();
        Q.pop();
        for (const BasicBlock *v : successors(*u)) {
            if (!S.contains(v)) {
                S.insert(v);
                Q.push(v);
            }
        }
    }

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    for (const BasicBlock *B : S) {
        for (const Instruction &I : *B) {
            for (const UseBase &use : uses(I)) {
                if (auto *II = dynamic_cast<const Instruction *>(use.user())) {
                    if (!S.contains(II->parent()) || (!dynamic_cast<const Phi *>(II) && !domTree.dominates(I, *II))) {
                        ok_ = false;
                        return;
                    }
                }
            }
        }
    }

    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (!dynamic_cast<const Phi *>(&I)) {
                for (const UseBase *op : I.operands()) {
                    if (&**op == &I) {
                        ok_ = false;
                        return;
                    }
                }
            }
        }
    }

    ok_ = true;
}

void VerificationAnalysis::runOnModule(const Module &M) {
    ok_ = true;
    for (const Function &F : M.functions) {
        if (!F.empty()) {
            runOnFunction(F);
            if (!ok_) {
                return;
            }
        }
    }
}
