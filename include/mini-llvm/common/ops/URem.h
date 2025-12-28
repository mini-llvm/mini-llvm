// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <concepts>
#include <optional>
#include <type_traits>

namespace mini_llvm::ops {

struct URem {
    template <typename T>
        requires std::integral<T>
    std::optional<T> operator()(T x, T y) const noexcept {
        if (y == 0)
            return std::nullopt;
        return
            std::bit_cast<T>(
                static_cast<std::make_unsigned_t<T>>(
                    std::bit_cast<std::make_unsigned_t<T>>(x) % std::bit_cast<std::make_unsigned_t<T>>(y)));
    }

    std::optional<bool> operator()(bool, bool y) const noexcept {
        if (y == false)
            return std::nullopt;
        return false;
    }
};

} // namespace mini_llvm::ops
