#pragma once

#include <bit>
#include <concepts>
#include <type_traits>

#include "mini-llvm/common/OpException.h"

namespace mini_llvm::ops {

template <typename To>
    requires std::integral<To>
struct Trunc {
    template <typename From>
        requires std::integral<From>
    To operator()(From) const {
        throw OpException();
    }

    template <typename From>
        requires std::integral<From> && (!std::same_as<From, bool>) && (sizeof(To) <= sizeof(From))
    To operator()(From x) const {
        return
            std::bit_cast<To>(
                static_cast<std::make_unsigned_t<To>>(
                    std::bit_cast<std::make_unsigned_t<From>>(x)));
    }
};

template <>
struct Trunc<bool> {
    template <typename From>
        requires std::integral<From>
    bool operator()(From x) const {
        return static_cast<bool>(x & 1);
    }

    bool operator()(bool x) const {
        return x;
    }
};

} // namespace mini_llvm::ops
