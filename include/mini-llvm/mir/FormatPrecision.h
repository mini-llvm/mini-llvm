// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>

#include "mini-llvm/common/Precision.h"

namespace mini_llvm::mir {

inline constexpr const char *specifier(Precision precision) {
    using enum Precision;
    switch (precision) {
        case kSingle: return "single";
        case kDouble: return "double";
        default: abort();
    }
}

} // namespace mini_llvm::mir
