#pragma once

#include <concepts>
#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterOperand.h"

namespace mini_llvm::mir {

class Instruction {
public:
    virtual ~Instruction() = default;
    Instruction() = default;
    Instruction(const Instruction &) = delete;
    Instruction(Instruction &&) = delete;
    Instruction &operator=(const Instruction &) = delete;
    Instruction &operator=(Instruction &&) = delete;
    virtual std::unordered_set<const RegisterOperand *> regOps() const = 0;
    virtual std::unordered_set<const RegisterOperand *> dsts() const = 0;
    virtual std::unordered_set<const RegisterOperand *> srcs() const = 0;
    std::unordered_set<RegisterOperand *> regOps();
    std::unordered_set<RegisterOperand *> srcs();
    std::unordered_set<RegisterOperand *> dsts();
    virtual std::unordered_set<PhysicalRegister *> implicitDsts() const { return {}; }
    virtual std::unordered_set<PhysicalRegister *> implicitSrcs() const { return {}; }
    virtual bool hasSideEffects() const = 0;
    virtual std::string format() const = 0;
    virtual std::unique_ptr<Instruction> clone() const = 0;
    virtual void accept(InstructionVisitor &visitor) = 0;
    virtual void accept(InstructionVisitor &visitor) const = 0;
};

std::unordered_set<Register *> use(const Instruction &I);
std::unordered_set<Register *> def(const Instruction &I);

} // namespace mini_llvm::mir

template <typename InstructionT>
    requires std::derived_from<InstructionT, mini_llvm::mir::Instruction>
struct std::formatter<InstructionT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const InstructionT &I, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", I.format());
    }
};
