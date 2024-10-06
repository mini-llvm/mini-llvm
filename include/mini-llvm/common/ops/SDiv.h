#pragma once

#include <bit>
#include <concepts>
#include <limits>
#include <type_traits>

#include "mini-llvm/common/OpException.h"

namespace mini_llvm::ops {

struct SDiv {
    template <typename T>
        requires std::integral<T>
    T operator()(T x, T y) const {
        std::make_signed_t<T> sx = std::bit_cast<std::make_signed_t<T>>(x),
                              sy = std::bit_cast<std::make_signed_t<T>>(y);
        if (sy == 0)
            throw OpException();
        if (sx == std::numeric_limits<std::make_signed_t<T>>::min() && sy == -1)
            throw OpException();
        return std::bit_cast<T>(static_cast<T>(sx / sy));
    }

    bool operator()(bool x, bool y) const {
        if (y == false)
            throw OpException();
        return x;
    }
};

} // namespace mini_llvm::ops
