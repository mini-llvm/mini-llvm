#pragma once

#include <concepts>
#include <format>
#include <string>

namespace mini_llvm::mc {

class Operand {
public:
    virtual ~Operand() = default;
    Operand() = default;
    Operand(const Operand &) = delete;
    Operand(Operand &&) = delete;
    Operand &operator=(const Operand &) = delete;
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

    auto format(const OperandT &line, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", line.format());
    }
};
