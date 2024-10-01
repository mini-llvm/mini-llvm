#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class LI : public Instruction {
public:
    LI(int width, std::shared_ptr<Register> dst, std::unique_ptr<Immediate> src)
        : width_(width), dst_(RegisterKind::kInteger, std::move(dst)), src_(std::move(src)) {}

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

    std::string format() const override {
        return std::format("LI<{}> {}, {}", width(), *dst(), *src());
    }

    bool hasSideEffects() const override {
        return false;
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<LI>(width(), share(*dst()), src()->clone());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLI(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLI(*this);
    }

private:
    int width_;
    RegisterOperand dst_;
    ImmediateOperand src_;
};

} // namespace mini_llvm::mir
