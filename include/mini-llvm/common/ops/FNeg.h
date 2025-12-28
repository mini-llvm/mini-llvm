// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

namespace mini_llvm::ops {

struct FNeg {
    template <typename T>
        requires std::floating_point<T>
    T operator()(T x) const noexcept {
        return -x;
    }
};

} // namespace mini_llvm::ops
