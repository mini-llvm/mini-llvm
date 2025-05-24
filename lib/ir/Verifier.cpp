#include "mini-llvm/ir/Verifier.h"

#include <iterator>
#include <memory>
#include <queue>
#include <unordered_set>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingOperator.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Instruction/FPExt.h"
#include "mini-llvm/ir/Instruction/FPTrunc.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/ICmp.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/SExt.h"
#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/Instruction/Trunc.h"
#include "mini-llvm/ir/Instruction/ZExt.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Type/BasicBlockType.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

bool checkOperands(const Instruction &I) {
    if (auto *op = dynamic_cast<const BinaryIntegerOperator *>(&I)) {
        if (*op->lhs()->type() != *op->rhs()->type()) {
            return false;
        }
        if (!dynamic_cast<const ICmp *>(&I) && *op->lhs()->type() == Ptr()) {
            return false;
        }
        return true;
    }
    if (auto *op = dynamic_cast<const BinaryFloatingOperator *>(&I)) {
        if (*op->lhs()->type() != *op->rhs()->type()) {
            return false;
        }
        if (*op->lhs()->type() == Ptr()) {
            return false;
        }
        return true;
    }
    if (auto *trunc = dynamic_cast<const Trunc *>(&I)) {
        if (*trunc->value()->type() == Ptr()) {
            return false;
        }
        if (*trunc->type() == Ptr()) {
            return false;
        }
        if (trunc->type()->bitSize() >= trunc->value()->type()->bitSize()) {
            return false;
        }
        return true;
    }
    if (auto *sext = dynamic_cast<const SExt *>(&I)) {
        if (*sext->value()->type() == Ptr()) {
            return false;
        }
        if (*sext->type() == Ptr()) {
            return false;
        }
        if (sext->type()->bitSize() <= sext->value()->type()->bitSize()) {
            return false;
        }
        return true;
    }
    if (auto *zext = dynamic_cast<const ZExt *>(&I)) {
        if (*zext->value()->type() == Ptr()) {
            return false;
        }
        if (*zext->type() == Ptr()) {
            return false;
        }
        if (zext->type()->bitSize() <= zext->value()->type()->bitSize()) {
            return false;
        }
        return true;
    }
    if (auto *fptrunc = dynamic_cast<const FPTrunc *>(&I)) {
        if (fptrunc->type()->bitSize() >= fptrunc->value()->type()->bitSize()) {
            return false;
        }
        return true;
    }
    if (auto *fpext = dynamic_cast<const FPExt *>(&I)) {
        if (fpext->type()->bitSize() <= fpext->value()->type()->bitSize()) {
            return false;
        }
        return true;
    }
    if (auto *select = dynamic_cast<const Select *>(&I)) {
        if (*select->trueValue()->type() != *select->falseValue()->type()) {
            return false;
        }
        return true;
    }
    if (auto *gep = dynamic_cast<const GetElementPtr *>(&I)) {
        if (*gep->sourceType() == Void() || *gep->sourceType() == BasicBlockType()) {
            return false;
        }
        return true;
    }
    return true;
}

bool checkDominance(const Function &F) {
    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

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

    for (const BasicBlock *B : S) {
        for (const Instruction &I : *B) {
            for (const UseBase &use : uses(I)) {
                if (auto *II = dynamic_cast<const Instruction *>(use.user())) {
                    if (!dynamic_cast<const Phi *>(II) && S.contains(II->parent()) && !domTree.dominates(I, *II)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool checkAlloca(const Function &F) {
    HashMap<const BasicBlock *, int> in;
    std::unordered_set<const BasicBlock *> S;
    std::queue<const BasicBlock *> Q;
    for (const BasicBlock &v : F) {
        in(&v) = 0;
    }
    for (const BasicBlock &u : F) {
        for (const BasicBlock *v : successors(u)) {
            ++in[v];
        }
    }
    for (const BasicBlock &v : F) {
        if (in[&v] == 0) {
            S.insert(&v);
            Q.push(&v);
        }
    }
    while (!Q.empty()) {
        const BasicBlock *u = Q.front();
        Q.pop();
        for (const BasicBlock *v : successors(*u)) {
            --in[v];
            if (in[v] == 0) {
                S.insert(v);
                Q.push(v);
            }
        }
    }
    for (const BasicBlock &B : F) {
        if (!S.contains(&B)) {
            for (const Instruction &I : B) {
                if (dynamic_cast<const Alloca *>(&I)) {
                    return false;
                }
            }
        }
    }
    return true;
}

} // namespace

bool ir::verifyFunction(const Function &F) {
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
        }
    }

    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (!checkOperands(I)) {
                return false;
            }
        }
    }

    if (!checkDominance(F)) {
        return false;
    }

    if (!checkAlloca(F)) {
        return false;
    }

    return true;
}

bool ir::verifyModule(const Module &M) {
    for (const Function &F : functions(M)) {
        if (!F.isDeclaration()) {
            if (!verifyFunction(F)) {
                return false;
            }
        }
    }

    return true;
}
