#include "mini-llvm/ir/Verify.h"

#include <iterator>
#include <memory>
#include <queue>
#include <unordered_set>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingOperator.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Instruction/FPExt.h"
#include "mini-llvm/ir/Instruction/FPTrunc.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/SExt.h"
#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/Instruction/Trunc.h"
#include "mini-llvm/ir/Instruction/ZExt.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

bool ir::verify(const Function &F) {
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            for (const UseBase *op : I.operands()) {
                if (op->expired()) {
                    return false;
                }
            }
        }
    }

    for (const BasicBlock &B : F) {
        if (B.empty()) {
            return false;
        }
        if (!dynamic_cast<const Terminator *>(&B.back())) {
            return false;
        }
        for (auto i = B.begin(), e = std::prev(B.end()); i != e; ++i) {
            if (dynamic_cast<const Terminator *>(&*i)) {
                return false;
            }
        }
    }

    for (const BasicBlock &B : F) {
        if (auto *ret = dynamic_cast<const Ret *>(&B.back())) {
            if (*ret->value()->type() != *F.functionType()->returnType()) {
                return false;
            }
        }
    }

    if (!hasNPredecessors(F.entry(), 0)) {
        return false;
    }

    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (auto *phi = dynamic_cast<const Phi *>(&I)) {
                if (incomingBlocks(*phi) != predecessors(B)) {
                    return false;
                }
            }
        }
    }

    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (!dynamic_cast<const Phi *>(&I)) {
                for (const UseBase *op : I.operands()) {
                    if (&**op == &I) {
                        return false;
                    }
                }
            }
            if (auto *op = dynamic_cast<const BinaryIntegerOperator *>(&I)) {
                if (*op->lhs()->type() != *op->rhs()->type()) {
                    return false;
                }
                if (*op->lhs()->type() == Ptr()) {
                    return false;
                }
            }
            if (auto *op = dynamic_cast<const BinaryFloatingOperator *>(&I)) {
                if (*op->lhs()->type() != *op->rhs()->type()) {
                    return false;
                }
                if (*op->lhs()->type() == Ptr()) {
                    return false;
                }
            }
            if (auto *trunc = dynamic_cast<const Trunc *>(&I)) {
                if (*trunc->value()->type() == Ptr()) {
                    return false;
                }
                if (*trunc->type() == Ptr()) {
                    return false;
                }
                if (trunc->type()->sizeInBits() >= trunc->value()->type()->sizeInBits()) {
                    return false;
                }
            }
            if (auto *sext = dynamic_cast<const SExt *>(&I)) {
                if (*sext->value()->type() == Ptr()) {
                    return false;
                }
                if (*sext->type() == Ptr()) {
                    return false;
                }
                if (sext->type()->sizeInBits() <= sext->value()->type()->sizeInBits()) {
                    return false;
                }
            }
            if (auto *zext = dynamic_cast<const ZExt *>(&I)) {
                if (*zext->value()->type() == Ptr()) {
                    return false;
                }
                if (*zext->type() == Ptr()) {
                    return false;
                }
                if (zext->type()->sizeInBits() <= zext->value()->type()->sizeInBits()) {
                    return false;
                }
            }
            if (auto *fptrunc = dynamic_cast<const FPTrunc *>(&I)) {
                if (fptrunc->type()->sizeInBits() >= fptrunc->value()->type()->sizeInBits()) {
                    return false;
                }
            }
            if (auto *fpext = dynamic_cast<const FPExt *>(&I)) {
                if (fpext->type()->sizeInBits() <= fpext->value()->type()->sizeInBits()) {
                    return false;
                }
            }
            if (auto *select = dynamic_cast<const Select *>(&I)) {
                if (*select->trueValue()->type() != *select->falseValue()->type()) {
                    return false;
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
                    if (!S.contains(II->parent())) {
                        return false;
                    }
                    if (!dynamic_cast<const Phi *>(II) && !domTree.dominates(I, *II)) {
                        return false;
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
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool ir::verify(const Module &M) {
    for (const Function &F : M.functions) {
        if (!F.empty()) {
            if (!verify(F)) {
                return false;
            }
        }
    }

    return true;
}
