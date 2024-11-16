#pragma once

#include <concepts>

#include "mini-llvm/common/PoisonValueException.h"

namespace mini_llvm::ops {

template <typename To>
    requires std::floating_point<To>
struct FPExt {
    template <typename From>
        requires std::floating_point<From>
    To operator()(From) const {
        throw PoisonValueException();
    }

    template <typename From>
        requires std::floating_point<From> && (sizeof(To) >= sizeof(From))
    To operator()(From x) const {
        return static_cast<To>(x);
    }
};

} // namespace mini_llvm::ops
