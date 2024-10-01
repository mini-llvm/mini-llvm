#pragma once

#include <climits>
#include <cmath>
#include <concepts>

#include "mini-llvm/common/OpException.h"

namespace mini_llvm::ops {

struct SHL {
    template <typename Tx, typename Ty>
        requires std::integral<Tx> && std::integral<Ty>
    Tx operator()(Tx x, Ty y) const {
        if (y >= static_cast<Ty>(sizeof(Tx) * CHAR_BIT))
            throw OpException();
        return static_cast<Tx>(x << y);
    }

    template <typename Ty>
        requires std::integral<Ty>
    bool operator()(bool x, Ty y) const {
        if (y != 0)
            throw OpException();
        return x;
    }
};

} // namespace mini_llvm::ops
