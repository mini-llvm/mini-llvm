#pragma once

#include <concepts>

namespace mini_llvm::ops {

struct FAdd {
    template <typename T>
        requires std::floating_point<T>
    T operator()(T x, T y) const {
        return x + y;
    }
};

} // namespace mini_llvm::ops
