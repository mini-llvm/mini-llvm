// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

namespace mini_llvm::ops {

struct FMul {
    template <typename T>
        requires std::floating_point<T>
    T operator()(T x, T y) const noexcept {
        return x * y;
    }
};

} // namespace mini_llvm::ops
