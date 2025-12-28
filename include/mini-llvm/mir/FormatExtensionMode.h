// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>

#include "mini-llvm/common/ExtensionMode.h"

namespace mini_llvm::mir {

inline constexpr const char *specifier(ExtensionMode mode) {
    using enum ExtensionMode;
    switch (mode) {
        case kNo: return "";
        case kSign: return "signext";
        case kZero: return "zeroext";
        default: abort();
    }
}

} // namespace mini_llvm::mir
