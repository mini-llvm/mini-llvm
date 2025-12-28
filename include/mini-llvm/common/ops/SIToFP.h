// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

namespace mini_llvm::ops {

template <typename To>
    requires std::floating_point<To>
struct SIToFP {
    template <typename From>
        requires std::integral<From>
    To operator()(From x) const noexcept {
        return static_cast<To>(std::bit_cast<std::make_signed_t<From>>(x));
    }

    To operator()(bool x) const noexcept {
        return static_cast<To>(x);
    }
};

} // namespace mini_llvm::ops
