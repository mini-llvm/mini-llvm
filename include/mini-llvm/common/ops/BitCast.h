#pragma once

#include <bit>
#include <concepts>
#include <utility>

namespace mini_llvm::ops {

template <typename To>
struct BitCast {
    template <typename From>
    To operator()(From) const noexcept {
        std::unreachable();
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
        std::unreachable();
    }

    bool operator()(bool x) const noexcept {
        return x;
    }
};

} // namespace mini_llvm::ops
