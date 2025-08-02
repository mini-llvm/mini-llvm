#pragma once

#include <bit>
#include <climits>
#include <cmath>
#include <concepts>
#include <optional>

namespace mini_llvm::ops {

struct SHL {
    template <typename Tx, typename Ty>
        requires std::integral<Tx> && std::integral<Ty>
    std::optional<Tx> operator()(Tx x, Ty y) const noexcept {
        std::make_unsigned_t<Ty> uy = std::bit_cast<std::make_unsigned_t<Ty>>(y);
        if (uy >= static_cast<std::make_unsigned_t<Ty>>(sizeof(Tx) * CHAR_BIT))
            return std::nullopt;
        return static_cast<Tx>(x << uy);
    }

    template <typename Ty>
        requires std::integral<Ty>
    std::optional<bool> operator()(bool x, Ty y) const noexcept {
        if (y != 0)
            return std::nullopt;
        return x;
    }
};

} // namespace mini_llvm::ops
