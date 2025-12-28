// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include "mini-llvm/common/ops/UGT.h"

namespace mini_llvm::ops {

struct ULE {
    template <typename T>
        requires std::integral<T>
    bool operator()(T x, T y) const noexcept {
        return !UGT()(x, y);
    }
};

} // namespace mini_llvm::ops
