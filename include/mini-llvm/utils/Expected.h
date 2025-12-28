// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

namespace mini_llvm {

template <typename E>
class Unexpected {
public:
    using error_type = E;

    constexpr Unexpected(E error) : error_(std::move(error)) {}

    template <typename F>
        requires std::is_convertible_v<F, E>
    constexpr Unexpected(F error) : error_(std::move(error)) {}

    template <typename F>
        requires std::is_convertible_v<F, E>
    constexpr Unexpected(Unexpected<F> error) : error_(*std::move(error)) {}

    constexpr E &operator*() & noexcept {
        return error_;
    }

    constexpr const E &operator*() const & noexcept {
        return error_;
    }

    constexpr E &&operator*() && noexcept {
        return std::move(error_);
    }

    constexpr E *operator->() noexcept {
        return std::addressof(error_);
    }

    constexpr const E *operator->() const noexcept {
        return std::addressof(error_);
    }

    constexpr E &error() & noexcept {
        return error_;
    }

    constexpr const E &error() const & noexcept {
        return error_;
    }

    constexpr E &&error() && noexcept {
        return std::move(error_);
    }

private:
    E error_;
};

template <typename T, typename E>
class [[nodiscard]] Expected {
public:
    using value_type = T;
    using error_type = E;

    constexpr Expected() = default;

    constexpr Expected(T value) : valueOrError_(std::move(value)) {}

    template <typename U>
        requires std::is_convertible_v<U, T>
    constexpr Expected(U value) : valueOrError_(static_cast<T>(std::move(value))) {}

    constexpr Expected(Unexpected<E> error) : valueOrError_(std::move(error)) {}

    template <typename F>
        requires std::is_convertible_v<F, E>
    constexpr Expected(Unexpected<F> error) : valueOrError_(static_cast<Unexpected<E>>(std::move(error))) {}

    explicit constexpr operator bool() const noexcept {
        return std::holds_alternative<T>(valueOrError_);
    }

    constexpr T &operator*() & noexcept {
        return std::get<T>(valueOrError_);
    }

    constexpr const T &operator*() const & noexcept {
        return std::get<T>(valueOrError_);
    }

    constexpr T &&operator*() && noexcept {
        return std::get<T>(std::move(valueOrError_));
    }

    constexpr T *operator->() noexcept {
        return std::addressof(std::get<T>(valueOrError_));
    }

    constexpr const T *operator->() const noexcept {
        return std::addressof(std::get<T>(valueOrError_));
    }

    constexpr T &value() & noexcept {
        return std::get<T>(valueOrError_);
    }

    constexpr const T &value() const & noexcept {
        return std::get<T>(valueOrError_);
    }

    constexpr T &&value() && noexcept {
        return std::get<T>(std::move(valueOrError_));
    }

    constexpr E &error() & noexcept {
        return *std::get<Unexpected<E>>(valueOrError_);
    }

    constexpr const E &error() const & noexcept {
        return *std::get<Unexpected<E>>(valueOrError_);
    }

    constexpr E &&error() && noexcept {
        return *std::get<Unexpected<E>>(std::move(valueOrError_));
    }

private:
    std::variant<T, Unexpected<E>> valueOrError_;
};

template <typename E>
class [[nodiscard]] Expected<void, E> {
public:
    constexpr Expected() : error_(std::nullopt) {}

    constexpr Expected(Unexpected<E> error) : error_(std::move(error)) {}

    template <typename F>
        requires std::is_convertible_v<F, E>
    constexpr Expected(Unexpected<F> error) : error_(static_cast<Unexpected<E>>(std::move(error))) {}

    explicit constexpr operator bool() const {
        return !error_;
    }

    constexpr E &error() & noexcept {
        return **error_;
    }

    constexpr const E &error() const & noexcept {
        return **error_;
    }

    constexpr E &&error() && noexcept {
        return **std::move(error_);
    }

private:
    std::optional<Unexpected<E>> error_;
};

} // namespace mini_llvm
