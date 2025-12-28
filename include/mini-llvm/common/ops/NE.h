// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

namespace mini_llvm::ops {

struct NE {
    template <typename T>
        requires std::integral<T>
    bool operator()(T x, T y) const noexcept {
        return x != y;
    }
};

} // namespace mini_llvm::ops
