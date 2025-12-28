// SPDX-License-Identifier: MIT

#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

namespace mini_llvm::ops {

struct UGT {
    template <typename T>
        requires std::integral<T>
    bool operator()(T x, T y) const noexcept {
        return std::bit_cast<std::make_unsigned_t<T>>(x) > std::bit_cast<std::make_unsigned_t<T>>(y);
    }

    bool operator()(bool x, bool y) const noexcept {
        return x && !y;
    }
};

} // namespace mini_llvm::ops
