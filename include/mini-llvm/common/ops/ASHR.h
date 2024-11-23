#pragma once

#include <bit>
#include <climits>
#include <concepts>
#include <optional>
#include <type_traits>

namespace mini_llvm::ops {

struct ASHR {
    template <typename Tx, typename Ty>
        requires std::integral<Tx> && std::integral<Ty>
    std::optional<Tx> operator()(Tx x, Ty y) const noexcept {
        if (y >= static_cast<Ty>(sizeof(Tx) * CHAR_BIT))
            return std::nullopt;
        return
            std::bit_cast<Tx>(
                static_cast<std::make_signed_t<Tx>>(
                    std::bit_cast<std::make_signed_t<Tx>>(x) >> y));
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
