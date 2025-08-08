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
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT FCvtFU : public Instruction {
public:
    FCvtFU(Precision dstPrecision, int srcWidth, std::shared_ptr<Register> dst, std::shared_ptr<Register> src)
        : dstPrecision_(dstPrecision),
          srcWidth_(srcWidth),
          dst_(RegisterClass::kFPR, std::move(dst)),
          src_(RegisterClass::kGPR, std::move(src)) {}

    Precision dstPrecision() const {
        return dstPrecision_;
    }

    int srcWidth() const {
        return srcWidth_;
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
        return {&dst(), &src()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {&dst()};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {&src()};
    }

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    bool hasSideEffects() const override {
        return true;
    }

    std::string format() const override {
        return std::format("FCVTFU {} i{} {}, {}", specifier(dstPrecision()), srcWidth() * 8, *dst(), *src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FCvtFU>(dstPrecision(), srcWidth(), share(*dst()), share(*src()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFCvtFU(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFCvtFU(*this);
    }

private:
    Precision dstPrecision_;
    int srcWidth_;
    RegisterOperand dst_, src_;
};

} // namespace mini_llvm::mir
