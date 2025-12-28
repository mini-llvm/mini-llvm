// SPDX-License-Identifier: MIT

#pragma once

#include <utility>

namespace mini_llvm {

template <typename T>
class ArrowProxy {
public:
    explicit constexpr ArrowProxy(T value)
        : value_(std::move(value)) {}

    template <typename... Args>
    explicit constexpr ArrowProxy(std::in_place_t, Args &&...args)
        : value_(std::forward<Args>(args)...) {}

    constexpr T *operator->() {
        return &value_;
    }

    template <typename... Args>
    static constexpr ArrowProxy<T> make(Args &&...args) {
        return ArrowProxy<T>(std::in_place, std::forward<Args>(args)...);
    }

private:
    T value_;
};

} // namespace mini_llvm
