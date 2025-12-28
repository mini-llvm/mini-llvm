// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/mc/RISCVAddress.h"

#include <format>
#include <string>

using namespace mini_llvm::mc;

std::string RISCVAddress::format() const {
    if (offset() == 0) {
        return std::format("{}", baseSymbol());
    }
    return std::format("{}{:+}", baseSymbol(), offset());
}
