// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <string>
#include <string_view>
#include <utility>

namespace mini_llvm {

class StringJoiner {
public:
    explicit constexpr StringJoiner(std::string delimiter)
        : delimiter_(std::move(delimiter)), first_(true) {}

    constexpr StringJoiner(std::string delimiter, std::string prefix, std::string suffix)
        : delimiter_(std::move(delimiter)), prefix_(std::move(prefix)), suffix_(std::move(suffix)), first_(true) {}

    constexpr StringJoiner &add(std::string_view element) {
        if (first_) {
            first_ = false;
        } else {
            out_ += delimiter_;
        }
        out_ += element;
        return *this;
    }

    template <typename... Args>
    constexpr StringJoiner &add(std::format_string<Args...> fmt, Args &&...args) {
        return add(std::vformat(fmt.get(), std::make_format_args(args...)));
    }

    constexpr std::string toString() const {
        return std::string(prefix_) + out_ + std::string(suffix_);
    }

private:
    std::string delimiter_, prefix_, suffix_;
    std::string out_;
    bool first_;
};

} // namespace mini_llvm

template <>
struct std::formatter<mini_llvm::StringJoiner> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::StringJoiner &joiner, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", joiner.toString());
    }
};
