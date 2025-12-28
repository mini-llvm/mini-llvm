// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <concepts>
#include <cstdlib>
#include <type_traits>

namespace mini_llvm::ops {

template <typename To>
    requires std::integral<To>
struct Trunc {
    template <typename From>
        requires std::integral<From>
    To operator()(From) const noexcept {
        abort();
    }

    template <typename From>
        requires (std::integral<From> && !std::same_as<From, bool> && sizeof(To) <= sizeof(From))
    To operator()(From x) const noexcept {
        return
            std::bit_cast<To>(
                static_cast<std::make_unsigned_t<To>>(
                    std::bit_cast<std::make_unsigned_t<From>>(x)));
    }
};

template <>
struct Trunc<bool> {
    template <typename From>
        requires std::integral<From>
    bool operator()(From x) const noexcept {
        return static_cast<bool>(x & 1);
    }

    bool operator()(bool x) const noexcept {
        return x;
    }
};

} // namespace mini_llvm::ops
