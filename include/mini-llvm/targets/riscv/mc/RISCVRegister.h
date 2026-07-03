// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib> // IWYU pragma: keep

namespace mini_llvm::mc {

// NOLINTNEXTLINE(cppcoreguidelines-use-enum-class)
enum RISCVRegister : int {
#define REGS
#define X(idx, name, class, width, isPreserved, isAllocatable) RISCV_##name = idx,
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS
};

constexpr const char *name(RISCVRegister reg) {
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
