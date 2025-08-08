// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <string>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

MINI_LLVM_EXPORT std::string toString(int32_t value, int base = 10);
MINI_LLVM_EXPORT std::string toString(int64_t value, int base = 10);
MINI_LLVM_EXPORT std::string toString(uint32_t value, int base = 10);
MINI_LLVM_EXPORT std::string toString(uint64_t value, int base = 10);

MINI_LLVM_EXPORT void normalizeLineEndings(std::string &str);

} // namespace mini_llvm
