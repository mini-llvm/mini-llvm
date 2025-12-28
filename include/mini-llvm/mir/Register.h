// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <format>
#include <memory>
#include <string>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT Register : public std::enable_shared_from_this<Register> {
public:
    virtual ~Register() = default;

    Register() = default;

    Register(const Register &) = delete;
    Register &operator=(const Register &) = delete;

    Register(Register &&) = delete;
    Register &operator=(Register &&) = delete;

    virtual int width() const = 0;
    virtual std::string format() const = 0;
};

} // namespace mini_llvm::mir

template <typename RegisterT>
    requires std::derived_from<RegisterT, mini_llvm::mir::Register>
struct std::formatter<RegisterT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const RegisterT &I, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", I.format());
    }
};
