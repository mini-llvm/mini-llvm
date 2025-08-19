// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/DeadCodeElimination.h"

#include <cassert>
#include <queue>
#include <unordered_set>

#include "mini-llvm/ir/Attribute/ReadNone.h"
#include "mini-llvm/ir/Attribute/ReadOnly.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingOperator.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Instruction/BitCast.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/FloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/FloatingToIntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/IntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/IntegerToFloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/IntToPtr.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/PtrToInt.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/UnaryFloatingOperator.h"
#include "mini-llvm/ir/Use.h"

using namespace mini_llvm::ir;

namespace {

bool isNonCriticalCall(const Instruction &I) {
    if (auto *call = dynamic_cast<const Call *>(&I)) {
        const Function &callee = *call->callee();
        if (callee.attr<ReadNone>() || callee.attr<ReadOnly>()) {
            return true;
        }
    }
    return false;
}

bool isCritical(const Instruction &I) {
    return !dynamic_cast<const BinaryIntegerOperator *>(&I)
        && !dynamic_cast<const BinaryFloatingOperator *>(&I)
        && !dynamic_cast<const UnaryFloatingOperator *>(&I)
        && !dynamic_cast<const IntegerCastingOperator *>(&I)
        && !dynamic_cast<const FloatingCastingOperator *>(&I)
        && !dynamic_cast<const IntegerToFloatingCastingOperator *>(&I)
        && !dynamic_cast<const FloatingToIntegerCastingOperator *>(&I)
        && !dynamic_cast<const PtrToInt *>(&I)
        && !dynamic_cast<const IntToPtr *>(&I)
        && !dynamic_cast<const BitCast *>(&I)
        && !dynamic_cast<const Select *>(&I)
        && !dynamic_cast<const GetElementPtr *>(&I)
        && !dynamic_cast<const Alloca *>(&I)
        && !dynamic_cast<const Load *>(&I)
        && !dynamic_cast<const Phi *>(&I)
        && !isNonCriticalCall(I);
}

} // namespace

bool DeadCodeElimination::runOnFunction(Function &F) {
    std::unordered_set<const Instruction *> visited;
    std::queue<const Instruction *> Q;
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (isCritical(I)) {
                visited.insert(&I);
                Q.push(&I);
            }
        }
    }
    while (!Q.empty()) {
        const Instruction *I = Q.front();
        Q.pop();
        for (const UseBase *op : I->operands()) {
            if (auto *II = dynamic_cast<const Instruction *>(&**op)) {
                if (visited.insert(II).second) {
                    Q.push(II);
                }
            }
        }
    }

    bool changed = false;

    for (const BasicBlock &B : F) {
        for (auto i = B.begin(); i != B.end();) {
            const Instruction &I = *i++;
            if (!visited.contains(&I)) {
                removeFromParent(I);
                changed = true;
            }
        }
    }

    assert(F.isWellFormed());
    return changed;
}
