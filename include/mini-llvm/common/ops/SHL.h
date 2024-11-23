#pragma once

#include <climits>
#include <cmath>
#include <concepts>
#include <optional>

namespace mini_llvm::ops {

struct SHL {
    template <typename Tx, typename Ty>
        requires std::integral<Tx> && std::integral<Ty>
    std::optional<Tx> operator()(Tx x, Ty y) const noexcept {
        if (y >= static_cast<Ty>(sizeof(Tx) * CHAR_BIT))
            return std::nullopt;
        return static_cast<Tx>(x << y);
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
