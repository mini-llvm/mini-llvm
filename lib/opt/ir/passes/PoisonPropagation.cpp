// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/PoisonPropagation.h"

#include <algorithm>
#include <cassert>
#include <memory>

#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingOperator.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Instruction/BitCast.h"
#include "mini-llvm/ir/Instruction/FloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/FloatingToIntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/IntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/IntegerToFloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/IntToPtr.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/PtrToInt.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Instruction/UnaryFloatingOperator.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

using namespace mini_llvm::ir;

namespace {

bool isPoison(const Instruction &I) {
    if (dynamic_cast<const BinaryIntegerOperator *>(&I)
            || dynamic_cast<const BinaryFloatingOperator *>(&I)
            || dynamic_cast<const UnaryFloatingOperator *>(&I)
            || dynamic_cast<const IntegerCastingOperator *>(&I)
            || dynamic_cast<const FloatingCastingOperator *>(&I)
            || dynamic_cast<const IntegerToFloatingCastingOperator *>(&I)
            || dynamic_cast<const FloatingToIntegerCastingOperator *>(&I)
            || dynamic_cast<const PtrToInt *>(&I)
            || dynamic_cast<const IntToPtr *>(&I)
            || dynamic_cast<const BitCast *>(&I)
            || dynamic_cast<const GetElementPtr *>(&I)
            || dynamic_cast<const Load *>(&I)) {
        return std::ranges::any_of(I.operands(), [](const UseBase *op) {
            return dynamic_cast<const PoisonValue *>(&**op);
        });
    }

    if (auto *select = dynamic_cast<const Select *>(&I)) {
        return dynamic_cast<const PoisonValue *>(&*select->cond());
    }

    return false;
}

void dfs(const DTNode *node, bool &changed) {
    for (auto i = node->block->begin(); i != node->block->end();) {
        const Instruction &I = *i++;
        if (isPoison(I)) {
            replaceAllUsesWith(I, std::make_shared<PoisonValue>(I.type()));
            removeFromParent(I);
            changed = true;
        }
    }

    for (const DTNode *child : node->children) {
        dfs(child, changed);
    }
}

} // namespace

bool PoisonPropagation::runOnFunction(Function &F) {
    bool changed = false;

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    dfs(domTree.node(F.entry()), changed);

    assert(F.isWellFormed());
    return changed;
}
