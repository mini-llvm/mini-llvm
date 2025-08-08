// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

MINI_LLVM_EXPORT char32_t decodeUtf8(const char *&ptr) noexcept;

} // namespace mini_llvm
