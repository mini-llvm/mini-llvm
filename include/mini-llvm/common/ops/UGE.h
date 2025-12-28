// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include "mini-llvm/common/ops/ULT.h"

namespace mini_llvm::ops {

struct UGE {
    template <typename T>
        requires std::integral<T>
    bool operator()(T x, T y) const noexcept {
        return !ULT()(x, y);
    }
};

} // namespace mini_llvm::ops
