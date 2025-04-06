#pragma once

#include <cstdlib>

#include "mini-llvm/common/ExtensionMode.h"

namespace mini_llvm::mir {

inline constexpr const char *specifier(ExtensionMode mode) {
    using enum ExtensionMode;
    switch (mode) {
        case kNo: return "nx";
        case kSign: return "sx";
        case kZero: return "zx";
        default: abort();
    }
}

} // namespace mini_llvm::mir
