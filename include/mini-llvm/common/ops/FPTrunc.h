// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <cstdlib>

namespace mini_llvm::ops {

template <typename To>
    requires std::floating_point<To>
struct FPTrunc {
    template <typename From>
        requires std::floating_point<From>
    To operator()(From) const noexcept {
        abort();
    }

    template <typename From>
        requires (std::floating_point<From> && sizeof(To) <= sizeof(From))
    To operator()(From x) const noexcept {
        return static_cast<To>(x);
    }
};

} // namespace mini_llvm::ops
