#pragma once

#include <utility>
#include <variant>

namespace mini_llvm {

template <typename T, typename E>
class Expected {
public:
    using value_type = T;
    using error_type = E;

    constexpr Expected(T value) : valueOrError_(std::move(value)) {}

    constexpr Expected(E error) : valueOrError_(std::move(error)) {}

    constexpr operator bool() const {
        return std::holds_alternative<T>(valueOrError_);
    }

    constexpr T &value() {
        return std::get<T>(valueOrError_);
    }

    constexpr const T &value() const {
        return std::get<T>(valueOrError_);
    }

    constexpr E &error() {
        return std::get<E>(valueOrError_);
    }

    constexpr const E &error() const {
        return std::get<E>(valueOrError_);
    }

private:
    std::variant<T, E> valueOrError_;
};

} // namespace mini_llvm
