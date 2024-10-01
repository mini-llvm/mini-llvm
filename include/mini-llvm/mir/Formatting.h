#pragma once

#include <utility>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/common/Precision.h"

namespace mini_llvm::mir {

constexpr const char *specifier(ExtensionMode mode) {
    using enum ExtensionMode;
    switch (mode) {
        case kNo: return "nx";
        case kSign: return "sx";
        case kZero: return "zx";
        default: std::unreachable();
    }
}

inline constexpr const char *specifier(Precision precision) {
    using enum Precision;
    switch (precision) {
        case kSingle: return "s";
        case kDouble: return "d";
        default: std::unreachable();
    }
}

} // namespace mini_llvm::mir
