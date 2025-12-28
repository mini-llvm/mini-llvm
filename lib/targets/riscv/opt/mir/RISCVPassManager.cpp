// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/opt/mir/RISCVPassManager.h"

#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/mir/passes/BasicBlockMerging.h"
#include "mini-llvm/opt/mir/passes/BasicBlockReordering.h"
#include "mini-llvm/opt/mir/passes/CopyPropagation.h"
#include "mini-llvm/opt/mir/passes/DeadCodeElimination.h"
#include "mini-llvm/opt/mir/passes/JumpThreading.h"
#include "mini-llvm/opt/mir/passes/NullOperationElimination.h"
#include "mini-llvm/opt/mir/passes/RegisterReuse.h"
#include "mini-llvm/opt/mir/passes/StackOffsetEvaluation.h"
#include "mini-llvm/opt/mir/passes/TailDuplication.h"
#include "mini-llvm/opt/mir/passes/UnreachableBlockElimination.h"
#include "mini-llvm/opt/mir/passes/ZeroRegisterReplacement.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"
#include "mini-llvm/targets/riscv/opt/mir/passes/RISCVConstantPropagation.h"

using namespace mini_llvm::mir;

void RISCVPassManager::runBeforeRegisterAllocation(Module &M) const {
    RISCVRegister *x0 = RISCVRegister::get("x0");

    bool changed;
    do {
        changed = false;

        changed |= NullOperationElimination().runOnModule(M);
        changed |= RISCVConstantPropagation().runOnModule(M);
        changed |= CopyPropagation().runOnModule(M);
        changed |= RegisterReuse().runOnModule(M);
        changed |= ZeroRegisterReplacement(x0).runOnModule(M);
        changed |= DeadCodeElimination().runOnModule(M);
        changed |= JumpThreading().runOnModule(M);
        changed |= BasicBlockMerging().runOnModule(M);
        changed |= TailDuplication(8).runOnModule(M);
        changed |= UnreachableBlockElimination().runOnModule(M);
    } while (changed);
}

void RISCVPassManager::runAfterRegisterAllocation(Module &M) const {
    RISCVRegister *x0 = RISCVRegister::get("x0");

    StackOffsetEvaluation().runOnModule(M);

    bool changed;
    do {
        changed = false;

        changed |= NullOperationElimination().runOnModule(M);
        changed |= RISCVConstantPropagation().runOnModule(M);
        changed |= CopyPropagation().runOnModule(M);
        changed |= RegisterReuse().runOnModule(M);
        changed |= ZeroRegisterReplacement(x0).runOnModule(M);
        changed |= DeadCodeElimination().runOnModule(M);
        changed |= JumpThreading().runOnModule(M);
        changed |= BasicBlockMerging().runOnModule(M);
        changed |= TailDuplication(8).runOnModule(M);
        changed |= UnreachableBlockElimination().runOnModule(M);
    } while (changed);

    BasicBlockReordering().runOnModule(M);
}
