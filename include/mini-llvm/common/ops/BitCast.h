#pragma once

#include <bit>
#include <concepts>

#include "mini-llvm/utils/Panic.h"

namespace mini_llvm::ops {

template <typename To>
struct BitCast {
    template <typename From>
    To operator()(From) const noexcept {
        panic();
    }

    template <typename From>
        requires (!std::same_as<From, bool>) && (sizeof(To) == sizeof(From))
    To operator()(From x) const noexcept {
        return std::bit_cast<To>(x);
    }
};

template <>
struct BitCast<bool> {
    template <typename From>
    bool operator()(From) const noexcept {
        panic();
    }

    bool operator()(bool x) const noexcept {
        return x;
    }
};

} // namespace mini_llvm::ops
