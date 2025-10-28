// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/PassManager.h"

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/passes/AlgebraicSimplification.h"
#include "mini-llvm/opt/ir/passes/ArrayFlattening.h"
#include "mini-llvm/opt/ir/passes/AttributeDeduction.h"
#include "mini-llvm/opt/ir/passes/BasicBlockMerging.h"
#include "mini-llvm/opt/ir/passes/BranchSimplification.h"
#include "mini-llvm/opt/ir/passes/ConstantFolding.h"
#include "mini-llvm/opt/ir/passes/DeadCodeElimination.h"
#include "mini-llvm/opt/ir/passes/DeadStoreElimination.h"
#include "mini-llvm/opt/ir/passes/FunctionInlining.h"
#include "mini-llvm/opt/ir/passes/GlobalCodeMotion.h"
#include "mini-llvm/opt/ir/passes/GlobalDeadCodeElimination.h"
#include "mini-llvm/opt/ir/passes/GlobalValueNumbering.h"
#include "mini-llvm/opt/ir/passes/InstructionCombining.h"
#include "mini-llvm/opt/ir/passes/JumpThreading.h"
#include "mini-llvm/opt/ir/passes/Mem2Reg.h"
#include "mini-llvm/opt/ir/passes/PoisonPropagation.h"
#include "mini-llvm/opt/ir/passes/RedundantLoadElimination.h"
#include "mini-llvm/opt/ir/passes/StrengthReduction.h"
#include "mini-llvm/opt/ir/passes/TailCallElimination.h"
#include "mini-llvm/opt/ir/passes/TailDuplication.h"
#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"

using namespace mini_llvm::ir;

void PassManager::run(Module &M) const {
    Mem2Reg().runOnModule(M);

    bool changed;
    do {
        changed = false;

        changed |= AttributeDeduction().runOnModule(M);
        changed |= DeadCodeElimination().runOnModule(M);
        changed |= BranchSimplification().runOnModule(M);
        changed |= JumpThreading().runOnModule(M);
        changed |= BasicBlockMerging().runOnModule(M);
        changed |= TailDuplication(5).runOnModule(M);
        changed |= UnreachableBlockElimination().runOnModule(M);
        changed |= TailCallElimination().runOnModule(M);
        changed |= DeadStoreElimination(pointerSize_).runOnModule(M);
        changed |= RedundantLoadElimination(pointerSize_).runOnModule(M);
        changed |= ArrayFlattening(pointerSize_).runOnModule(M);
        changed |= InstructionCombining().runOnModule(M);
        changed |= AlgebraicSimplification().runOnModule(M);
        changed |= ConstantFolding().runOnModule(M);
        changed |= PoisonPropagation().runOnModule(M);
        changed |= GlobalCodeMotion().runOnModule(M);
        changed |= GlobalValueNumbering().runOnModule(M);
        changed |= StrengthReduction(3, 20, 20).runOnModule(M);
        changed |= FunctionInlining(20, 100).runOnModule(M);
        changed |= GlobalDeadCodeElimination().runOnModule(M);
    } while (changed);
}
