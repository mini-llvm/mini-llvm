// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <format>
#include <string>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT Operand {
public:
    virtual ~Operand() = default;

    Operand() = default;

    Operand(const Operand &) = delete;
    Operand &operator=(const Operand &) = delete;

    Operand(Operand &&) = delete;
    Operand &operator=(Operand &&) = delete;

    virtual std::string format() const = 0;
};

} // namespace mini_llvm::mc

template <typename OperandT>
    requires std::derived_from<OperandT, mini_llvm::mc::Operand>
struct std::formatter<OperandT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const OperandT &operand, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", operand.format());
    }
};
