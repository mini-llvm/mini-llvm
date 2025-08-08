// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/mir/FormatPrecision.h"
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

class MINI_LLVM_EXPORT FLoad : public Instruction {
public:
    FLoad(Precision precision, std::shared_ptr<Register> dst, MemoryOperand src)
        : precision_(precision),
          dst_(RegisterClass::kFPR, std::move(dst)),
          src_(std::move(src)) {}

    Precision precision() const {
        return precision_;
    }

    template <typename Self>
    auto &dst(this Self &&self) {
        return self.dst_;
    }

    template <typename Self>
    auto &src(this Self &&self) {
        return self.src_;
    }

    std::unordered_set<const RegisterOperand *> regOps() const override {
        return {&dst(), &src().baseReg()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {&dst()};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {&src().baseReg()};
    }

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {&src().offset()};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {&src()};
    }

    bool hasSideEffects() const override {
        return true;
    }

    std::string format() const override {
        return std::format("FLOAD {} {}, {}", specifier(precision()), *dst(), src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FLoad>(precision(), share(*dst()), src().clone());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFLoad(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFLoad(*this);
    }

private:
    Precision precision_;
    RegisterOperand dst_;
    MemoryOperand src_;
};

} // namespace mini_llvm::mir
