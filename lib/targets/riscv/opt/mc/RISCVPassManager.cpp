// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/opt/mc/RISCVPassManager.h"

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVFallthrough.h"
#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVUnusedLabelElimination.h"

using namespace mini_llvm::mc;

void RISCVPassManager::run(Module &M) const {
    RISCVFallthrough().runOnModule(M);
    RISCVUnusedLabelElimination().runOnModule(M);
}
