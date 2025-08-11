// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <optional>
#include <type_traits>
#include <utility>

namespace mini_llvm {

template <typename T>
class OptionalRef {
    static_assert(std::is_object_v<T>);

public:
    using value_type = T;

    constexpr OptionalRef() noexcept : value_(nullptr) {}

    constexpr OptionalRef(std::nullopt_t) noexcept : value_(nullptr) {}

    template <typename U>
        requires std::is_convertible_v<U &, T &>
    constexpr OptionalRef(U &value) noexcept : value_(&value) {}

    constexpr OptionalRef(const OptionalRef &other) noexcept = default;

    template <typename U>
        requires std::is_convertible_v<U &, T &>
    constexpr OptionalRef(const OptionalRef<U> &other) noexcept : value_(other.value_) {}

    constexpr OptionalRef &operator=(const OptionalRef &other) noexcept = default;

    explicit constexpr operator bool() const noexcept {
        return value_ != nullptr;
    }

    constexpr bool has_value() const noexcept {
        return value_ != nullptr;
    }

    constexpr T *operator->() const noexcept {
        assert(value_ != nullptr);
        return value_;
    }

    constexpr T &operator*() const noexcept {
        assert(value_ != nullptr);
        return *value_;
    }

    constexpr T &value() const {
        if (!value_) {
            throw std::bad_optional_access();
        }
        return *value_;
    }

    template <typename U = std::remove_cv_t<T>>
        requires std::is_convertible_v<U &&, std::remove_cv_t<T>>
    constexpr std::remove_cv_t<T> value_or(U &&defaultValue) const {
        if (!value_) {
            return static_cast<std::remove_cv_t<T>>(std::forward<U>(defaultValue));
        }
        return *value_;
    }

    constexpr void reset() noexcept {
        value_ = nullptr;
    }

    constexpr void swap(OptionalRef &other) noexcept {
        std::swap(value_, other.value_);
    }

private:
    T *value_;

    template <typename> friend class OptionalRef;
};

template <typename T>
OptionalRef(T &) -> OptionalRef<T>;

template <typename T>
void swap(OptionalRef<T> &lhs, OptionalRef<T> &rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace mini_llvm
