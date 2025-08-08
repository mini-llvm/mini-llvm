// SPDX-License-Identifier: MIT

#include "mini-llvm/mc/Address.h"

#include <format>
#include <string>

using namespace mini_llvm::mc;

std::string Address::format() const {
    if (offset() == 0) {
        return std::format("{}", baseSymbol());
    }
    return std::format("{}{:+}", baseSymbol(), offset());
}
