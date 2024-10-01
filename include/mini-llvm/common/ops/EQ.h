#pragma once

#include <concepts>

namespace mini_llvm::ops {

struct EQ {
    template <typename T>
        requires std::integral<T>
    bool operator()(T x, T y) const {
        return x == y;
    }
};

} // namespace mini_llvm::ops
