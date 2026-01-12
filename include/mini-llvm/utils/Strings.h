// SPDX-License-Identifier: MIT

#pragma once

#include <string>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

MINI_LLVM_EXPORT std::string toString(int value, int base = 10);
MINI_LLVM_EXPORT std::string toString(long value, int base = 10);
MINI_LLVM_EXPORT std::string toString(long long value, int base = 10);
MINI_LLVM_EXPORT std::string toString(unsigned int value, int base = 10);
MINI_LLVM_EXPORT std::string toString(unsigned long value, int base = 10);
MINI_LLVM_EXPORT std::string toString(unsigned long long value, int base = 10);

MINI_LLVM_EXPORT void normalizeLineEndings(std::string &str);

} // namespace mini_llvm
