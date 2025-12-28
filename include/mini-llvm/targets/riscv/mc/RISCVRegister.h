// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>

namespace mini_llvm::mc {

enum RISCVRegister {
#define REGS
#define X(idx, name, class, width, isPreserved, isAllocatable) RISCV_##name = idx,
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS
};

inline constexpr const char *name(RISCVRegister reg) {
    switch (reg) {
#define REGS
#define X(idx, name, class, width, isPreserved, isAllocatable) case RISCV_##name: return #name;
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS
    default:
        abort();
    }
}

} // namespace mini_llvm::mc
