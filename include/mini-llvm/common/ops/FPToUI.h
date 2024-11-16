#pragma once

#include <bit>
#include <cmath>
#include <concepts>
#include <limits>
#include <type_traits>

#include "mini-llvm/common/PoisonValueException.h"

namespace mini_llvm::ops {

template <typename To>
    requires std::integral<To>
struct FPToUI {
    template <typename From>
        requires std::floating_point<From>
    To operator()(From x) const {
        From t = std::trunc(x);
        if (t > static_cast<From>(std::numeric_limits<std::make_unsigned_t<To>>::max()))
            throw PoisonValueException();
        if (t < 0.0)
            throw PoisonValueException();
        return std::bit_cast<To>(static_cast<std::make_unsigned_t<To>>(x));
    }
};

template <>
struct FPToUI<bool> {
    template <typename From>
        requires std::floating_point<From>
    bool operator()(From x) const {
        From t = std::trunc(x);
        if (t == 0.0) return false;
        if (t == 1.0) return true;
        throw PoisonValueException();
    }
};

} // namespace mini_llvm::ops
