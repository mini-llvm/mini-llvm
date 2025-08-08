// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include "mini-llvm/common/ops/SLT.h"

namespace mini_llvm::ops {

struct SGE {
    template <typename T>
        requires std::integral<T>
    bool operator()(T x, T y) const noexcept {
        return !SLT()(x, y);
    }
};

} // namespace mini_llvm::ops
