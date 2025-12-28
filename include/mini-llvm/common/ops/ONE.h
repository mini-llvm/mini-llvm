// SPDX-License-Identifier: MIT

#pragma once

#include <cmath>
#include <concepts>

namespace mini_llvm::ops {

struct ONE {
    template <typename T>
        requires std::floating_point<T>
    bool operator()(T x, T y) const noexcept {
        return !std::isunordered(x, y) && x != y;
    }
};

} // namespace mini_llvm::ops
