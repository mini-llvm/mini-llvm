// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>

namespace mini_llvm::mc {

enum RISCVOperation {
#define OPS
#define X(mnemonic, name) RISCV_##name,
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef OPS
};

inline constexpr const char *mnemonic(RISCVOperation op) {
    switch (op) {
#define OPS
#define X(mnemonic, name) case RISCV_##name: return mnemonic;
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef OPS
    default:
        abort();
    }
}

} // namespace mini_llvm::mc
