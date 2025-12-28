// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <string_view>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

MINI_LLVM_EXPORT bool shouldQuoteDotId(std::string_view id);
MINI_LLVM_EXPORT std::string quoteDotId(std::string_view id);
MINI_LLVM_EXPORT std::string formatDotId(std::string_view id);

} // namespace mini_llvm
