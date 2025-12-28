// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <format>
#include <iterator>
#include <string>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT GlobalValue {
public:
    virtual ~GlobalValue() = default;

    GlobalValue() = default;

    GlobalValue(const GlobalValue &) = delete;
    GlobalValue &operator=(const GlobalValue &) = delete;

    GlobalValue(GlobalValue &&) = delete;
    GlobalValue &operator=(GlobalValue &&) = delete;

    virtual std::string name() const = 0;
    virtual Linkage linkage() const = 0;

    virtual std::string format() const = 0;

    std::string formatAsOperand() const;
};

} // namespace mini_llvm::mir

template <typename GlobalValueT>
    requires std::derived_from<GlobalValueT, mini_llvm::mir::GlobalValue>
struct std::formatter<GlobalValueT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        if (*ctx.begin() == 'o') {
            asOperand_ = true;
            return std::next(ctx.begin());
        }
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const GlobalValueT &value, FormatContext &ctx) const {
        if (asOperand_) {
            return std::format_to(ctx.out(), "{}", value.formatAsOperand());
        }
        return std::format_to(ctx.out(), "{}", value.format());
    }

private:
    bool asOperand_ = false;
};
