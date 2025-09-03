// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

namespace mini_llvm::ops {

template <typename To>
    requires std::integral<To>
struct FPToSI {
    template <typename From>
        requires std::floating_point<From>
    std::optional<To> operator()(From x) const noexcept {
        if (std::isnan(x))
            return std::nullopt;
        From t = std::trunc(x);
        if (t > static_cast<From>(std::numeric_limits<std::make_signed_t<To>>::max()))
            return std::nullopt;
        if (t < static_cast<From>(std::numeric_limits<std::make_signed_t<To>>::min()))
            return std::nullopt;
        return std::bit_cast<To>(static_cast<std::make_signed_t<To>>(x));
    }
};

template <>
struct FPToSI<bool> {
    template <typename From>
        requires std::floating_point<From>
    std::optional<bool> operator()(From x) const noexcept {
        From t = std::trunc(x);
        if (t == 0.0) return false;
        if (t == -1.0) return true;
        return std::nullopt;
    }
};

} // namespace mini_llvm::ops
