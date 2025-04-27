#pragma once

#include <format>
#include <memory>
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
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class FMovFI : public Instruction {
public:
    FMovFI(Precision precision, std::shared_ptr<Register> dst, std::shared_ptr<Register> src)
        : precision_(precision),
          dst_(RegisterClass::kFPR, std::move(dst)),
          src_(RegisterClass::kGPR, std::move(src)) {}

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
        return false;
    }

    std::string format() const override {
        return std::format("FMOVFI({}) {}, {}", specifier(precision()), *dst(), *src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FMovFI>(precision(), share(*dst()), share(*src()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFMovFI(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFMovFI(*this);
    }

private:
    Precision precision_;
    RegisterOperand dst_, src_;
};

} // namespace mini_llvm::mir
