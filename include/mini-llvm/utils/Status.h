#pragma once

#include <optional>
#include <utility>

namespace mini_llvm {

struct Ok {};

template <typename E>
class Status {
public:
    using error_type = E;

    constexpr Status(Ok) : error_(std::nullopt) {}

    constexpr Status(E error) : error_(std::move(error)) {}

    constexpr operator bool() const {
        return !error_.has_value();
    }

    constexpr E &error() {
        return error_.value();
    }

    constexpr const E &error() const {
        return error_.value();
    }

private:
    std::optional<E> error_;
};

} // namespace mini_llvm
