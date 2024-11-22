#pragma once

#include <utility>

namespace mini_llvm::mc {

enum RISCVOperation {
#define OPS
#define X(opcode, mnemonic, name) RISCV_##name = opcode,
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef OPS
};

constexpr const char *mnemonic(RISCVOperation op) {
    switch (op) {
#define OPS
#define X(opcode, mnemonic, name) case RISCV_##name: return mnemonic;
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef OPS
    default:
        std::unreachable();
    }
}

} // namespace mini_llvm::mc
