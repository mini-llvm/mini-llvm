#pragma once

#include <utility>

namespace mini_llvm::mc {

enum RISCVOperation {
#define OPS
#define X(opcode, mnemonic, enumerator) RISCV_##enumerator = opcode,
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef OPS
};

constexpr const char *mnemonic(RISCVOperation op) {
    switch (op) {
#define OPS
#define X(opcode, mnemonic, enumerator) case RISCV_##enumerator: return mnemonic;
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef OPS
    default:
        std::unreachable();
    }
}

} // namespace mini_llvm::mc
