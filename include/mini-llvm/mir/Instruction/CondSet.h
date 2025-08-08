// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT CondSet : public Instruction {
public:
    CondSet(int width, Condition cond, std::shared_ptr<Register> dst)
        : width_(width), cond_(cond), dst_(RegisterClass::kGPR, std::move(dst)) {}

    int width() const {
        return width_;
    }

    Condition cond() const {
        return cond_;
    }

    void setCond(Condition cond) {
        cond_ = cond;
    }

    template <typename Self>
    auto &dst(this Self &&self) {
        return self.dst_;
    }

    std::unordered_set<const RegisterOperand *> regOps() const override {
        return {&dst()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {&dst()};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {};
    }

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    bool hasSideEffects() const override {
        return false;
    }

    std::string format() const override {
        return std::format("CONDSET i{} {} {}", width() * 8, specifier(cond()), *dst());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<CondSet>(width(), cond(), share(*dst()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitCondSet(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitCondSet(*this);
    }

private:
    int width_;
    Condition cond_;
    RegisterOperand dst_;
};

} // namespace mini_llvm::mir
