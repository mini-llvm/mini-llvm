#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/mir/Formatting.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class Load : public Instruction {
public:
    Load(int width,
         std::shared_ptr<Register> dst,
         MemoryOperand src,
         ExtensionMode extensionMode = ExtensionMode::kNo)
        : width_(width),
          dst_(RegisterKind::kInteger, std::move(dst)),
          src_(std::move(src)),
          extensionMode_(extensionMode) {}

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

    ExtensionMode extensionMode() const {
        return extensionMode_;
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

    bool hasSideEffects() const override {
        return true;
    }

    std::string format() const override {
        return std::format(
            "LOAD<{}> {}, {}, {}",
            width(), *dst(), src(), specifier(extensionMode()));
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Load>(width(), share(*dst()), src().clone(), extensionMode());
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
    ExtensionMode extensionMode_;
};

} // namespace mini_llvm::mir
