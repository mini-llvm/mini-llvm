// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/opt/mc/RISCVPassManager.h"

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVFallthrough.h"
#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVUnusedLabelElimination.h"

namespace mini_llvm::mc {

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void RISCVPassManager::run(Module &M) const {
    RISCVFallthrough().runOnModule(M);
    RISCVUnusedLabelElimination().runOnModule(M);
}

} // namespace mini_llvm::mc
