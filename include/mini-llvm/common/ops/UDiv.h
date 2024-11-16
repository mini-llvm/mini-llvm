#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

#include "mini-llvm/common/PoisonValueException.h"

namespace mini_llvm::ops {

struct UDiv {
    template <typename T>
        requires std::integral<T>
    T operator()(T x, T y) const {
        if (y == 0)
            throw PoisonValueException();
        return
            std::bit_cast<T>(
                static_cast<std::make_unsigned_t<T>>(
                    std::bit_cast<std::make_unsigned_t<T>>(x) / std::bit_cast<std::make_unsigned_t<T>>(y)));
    }

    bool operator()(bool x, bool y) const {
        if (y == false)
            throw PoisonValueException();
        return x;
    }
};

} // namespace mini_llvm::ops
