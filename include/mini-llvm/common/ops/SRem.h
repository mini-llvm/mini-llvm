// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <concepts>
#include <limits>
#include <optional>
#include <type_traits>

namespace mini_llvm::ops {

struct SRem {
    template <typename T>
        requires std::integral<T>
    std::optional<T> operator()(T x, T y) const noexcept {
        std::make_signed_t<T> sx = std::bit_cast<std::make_signed_t<T>>(x),
                              sy = std::bit_cast<std::make_signed_t<T>>(y);
        if (sy == 0)
            return std::nullopt;
        if (sx == std::numeric_limits<std::make_signed_t<T>>::min() && sy == -1)
            return std::nullopt;
        return std::bit_cast<T>(static_cast<std::make_signed_t<T>>(sx % sy));
    }

    std::optional<bool> operator()(bool, bool y) const noexcept {
        if (y == false)
            return std::nullopt;
        return false;
    }
};

} // namespace mini_llvm::ops
