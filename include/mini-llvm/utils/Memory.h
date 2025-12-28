// SPDX-License-Identifier: MIT

#pragma once

#include <memory>

namespace mini_llvm {

template <typename To, typename From>
std::unique_ptr<To> cast(std::unique_ptr<From> from) noexcept {
    return std::unique_ptr<To>(static_cast<To *>(from.release()));
}

template <typename To, typename From>
std::shared_ptr<To> cast(std::shared_ptr<From> from) noexcept {
    return std::static_pointer_cast<To>(from);
}

template <typename To, typename From>
std::weak_ptr<To> cast(std::weak_ptr<From> from) noexcept {
    return std::static_pointer_cast<To>(from.lock());
}

template <typename T>
std::shared_ptr<T> share(T &value) {
    return cast<T>(value.shared_from_this());
}

template <typename T>
std::weak_ptr<T> weaken(T &value) {
    return cast<T>(value.weak_from_this());
}

} // namespace mini_llvm
