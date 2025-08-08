// SPDX-License-Identifier: MIT

#pragma once

#ifdef _WIN32

#include <string>
#include <string_view>
#include <vector>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::windows {

// https://utf8everywhere.org/#windows

MINI_LLVM_EXPORT std::wstring widen(std::string_view str);
MINI_LLVM_EXPORT std::string narrow(std::wstring_view wstr);

} // namespace mini_llvm::windows

#endif // _WIN32
