// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/common/ExtensionMode.h"
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

class MINI_LLVM_EXPORT Load : public Instruction {
public:
    Load(int width,
         std::shared_ptr<Register> dst,
         MemoryOperand src,
         ExtensionMode extMode = ExtensionMode::kNo)
        : width_(width),
          dst_(RegisterClass::kGPR, std::move(dst)),
          src_(std::move(src)),
          extMode_(extMode) {}

    int width() const {
        return width_;
    }

    template <typename Self>
    auto &dst(this Self &&self) {
        return self.dst_;
    }

    template <typename Self>
    auto &src(this Self &&self) {
        return self.src_;
    }

    ExtensionMode extMode() const {
        return extMode_;
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

    std::string format() const override;

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Load>(width(), share(*dst()), src().clone(), extMode());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLoad(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLoad(*this);
    }

private:
    int width_;
    RegisterOperand dst_;
    MemoryOperand src_;
    ExtensionMode extMode_;
};

} // namespace mini_llvm::mir
