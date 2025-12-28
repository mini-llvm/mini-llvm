// SPDX-License-Identifier: MIT

#pragma once

namespace mini_llvm::ops {

struct Or {
    template <typename T>
    T operator()(T x, T y) const noexcept {
        return x | y;
    }

    bool operator()(bool x, bool y) const noexcept {
        return x || y;
    }
};

} // namespace mini_llvm::ops
