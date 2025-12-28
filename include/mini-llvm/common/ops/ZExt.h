// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <concepts>
#include <cstdlib>
#include <type_traits>

namespace mini_llvm::ops {

template <typename To>
    requires std::integral<To>
struct ZExt {
    template <typename From>
        requires std::integral<From>
    To operator()(From) const noexcept {
        abort();
    }

    template <typename From>
        requires (std::integral<From> && !std::same_as<From, bool> && sizeof(To) >= sizeof(From))
    To operator()(From x) const noexcept {
        return std::bit_cast<To>(static_cast<std::make_unsigned_t<To>>(std::bit_cast<std::make_unsigned_t<From>>(x)));
    }

    To operator()(bool x) const noexcept {
        return static_cast<To>(x);
    }
};

template <>
struct ZExt<bool> {
    template <typename From>
        requires std::integral<From>
    bool operator()(From) const noexcept {
        abort();
    }

    bool operator()(bool x) const noexcept {
        return x;
    }
};

} // namespace mini_llvm::ops
