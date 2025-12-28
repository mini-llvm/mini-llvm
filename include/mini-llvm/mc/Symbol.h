// SPDX-License-Identifier: MIT

#pragma once

#include <compare>
#include <cstddef>
#include <format>
#include <functional>
#include <string>
#include <utility>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT Symbol final {
public:
    explicit Symbol(std::string name) : name_(std::move(name)) {}

    const std::string &name() const & {
        return name_;
    }

    std::string &&name() && {
        return std::move(name_);
    }

    std::string format() const;

private:
    std::string name_;
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
    return lhs.name() == rhs.name();
}

inline std::strong_ordering operator<=>(const Symbol &lhs, const Symbol &rhs) {
    return lhs.name() <=> rhs.name();
}

} // namespace mini_llvm::mc

template <>
struct std::hash<mini_llvm::mc::Symbol> {
    size_t operator()(const mini_llvm::mc::Symbol &symbol) const {
        return std::hash<std::string>()(symbol.name());
    }
};

template <>
struct std::formatter<mini_llvm::mc::Symbol> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::mc::Symbol &symbol, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", symbol.format());
    }
};
