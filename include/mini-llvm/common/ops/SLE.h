// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include "mini-llvm/common/ops/SGT.h"

namespace mini_llvm::ops {

struct SLE {
    template <typename T>
        requires std::integral<T>
    bool operator()(T x, T y) const noexcept {
        return !SGT()(x, y);
    }
};

} // namespace mini_llvm::ops
