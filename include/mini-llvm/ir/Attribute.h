// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Attribute {
public:
    virtual ~Attribute() = default;

    Attribute(const Attribute &) = delete;
    Attribute &operator=(const Attribute &) = delete;

    Attribute(Attribute &&) = delete;
    Attribute &operator=(Attribute &&) = delete;

    virtual std::string format() const = 0;
    virtual std::unique_ptr<Attribute> clone() const = 0;

protected:
    Attribute() = default;
};

} // namespace mini_llvm::ir

template <>
struct std::formatter<mini_llvm::ir::Attribute> {
public:
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::ir::Attribute &attr, FormatContext &ctx) const {
        return format_to(ctx.out(), "{}", attr.format());
    }
};
