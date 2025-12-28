// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT Instruction {
public:
    virtual ~Instruction() = default;

    Instruction() = default;

    Instruction(const Instruction &) = delete;
    Instruction &operator=(const Instruction &) = delete;

    Instruction(Instruction &&) = delete;
    Instruction &operator=(Instruction &&) = delete;

    virtual std::unordered_set<const RegisterOperand *> regOps() const = 0;
    virtual std::unordered_set<const RegisterOperand *> dsts() const = 0;
    virtual std::unordered_set<const RegisterOperand *> srcs() const = 0;
    std::unordered_set<RegisterOperand *> regOps();
    std::unordered_set<RegisterOperand *> dsts();
    std::unordered_set<RegisterOperand *> srcs();
    virtual std::unordered_set<PhysicalRegister *> implicitDsts() const { return {}; }
    virtual std::unordered_set<PhysicalRegister *> implicitSrcs() const { return {}; }
    virtual std::unordered_set<const ImmediateOperand *> immOps() const = 0;
    std::unordered_set<ImmediateOperand *> immOps();
    virtual std::unordered_set<const MemoryOperand *> memOps() const = 0;
    std::unordered_set<MemoryOperand *> memOps();
    virtual bool isCoalescent() const { return false; }
    virtual Register *coalescenceDst() const { return nullptr; }
    virtual Register *coalescenceSrc() const { return nullptr; }
    virtual bool hasSideEffects() const = 0;
    virtual std::string format() const = 0;
    virtual std::unique_ptr<Instruction> clone() const = 0;
    virtual void accept(InstructionVisitor &visitor) = 0;
    virtual void accept(InstructionVisitor &visitor) const = 0;
};

MINI_LLVM_EXPORT std::unordered_set<Register *> use(const Instruction &I);
MINI_LLVM_EXPORT std::unordered_set<Register *> def(const Instruction &I);

} // namespace mini_llvm::mir

template <typename InstructionT>
    requires std::derived_from<InstructionT, mini_llvm::mir::Instruction>
struct std::formatter<InstructionT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const InstructionT &I, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", I.format());
    }
};
