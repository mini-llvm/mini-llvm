// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <format>
#include <string>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT Statement {
public:
    virtual ~Statement() = default;

    Statement() = default;

    Statement(const Statement &) = delete;
    Statement &operator=(const Statement &) = delete;

    Statement(Statement &&) = delete;
    Statement &operator=(Statement &&) = delete;

    virtual std::string format() const = 0;
};

} // namespace mini_llvm::mc

template <typename StatementT>
    requires std::derived_from<StatementT, mini_llvm::mc::Statement>
struct std::formatter<StatementT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const StatementT &stmt, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", stmt.format());
    }
};
