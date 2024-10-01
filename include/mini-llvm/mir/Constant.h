#pragma once

#include <concepts>
#include <format>
#include <string>

#include "mini-llvm/mir/ConstantVisitor.h"

namespace mini_llvm::mir {

class Constant {
public:
    virtual ~Constant() = default;
    virtual int size() const = 0;
    virtual std::string format() const = 0;
    virtual void accept(ConstantVisitor &visitor) = 0;
    virtual void accept(ConstantVisitor &visitor) const = 0;
};

} // namespace mini_llvm::mir

template <typename ConstantT>
    requires std::derived_from<ConstantT, mini_llvm::mir::Constant>
struct std::formatter<ConstantT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const ConstantT &C, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", C.format());
    }
};
