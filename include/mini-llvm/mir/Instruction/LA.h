// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/GlobalValue.h"
#include "mini-llvm/mir/GlobalValueOperand.h"
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

class MINI_LLVM_EXPORT LA : public Instruction {
public:
    LA(int width, std::shared_ptr<Register> dst, GlobalValue *src)
        : width_(width), dst_(RegisterClass::kGPR, std::move(dst)), src_(src) {}

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
        return std::format("LA i{} {}, {:o}", width() * 8, *dst(), *src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<LA>(width(), share(*dst()), &*src());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLA(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLA(*this);
    }

private:
    int width_;
    RegisterOperand dst_;
    GlobalValueOperand src_;
};

} // namespace mini_llvm::mir
