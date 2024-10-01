#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/UnaryOperator.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class Not : public UnaryOperator {
public:
    Not(int width,
        std::shared_ptr<Register> dst,
        std::shared_ptr<Register> src,
        ExtensionMode extensionMode = ExtensionMode::kNo)
        : UnaryOperator(width, std::move(dst), std::move(src), extensionMode) {}

    bool hasSideEffects() const override {
        return false;
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Not>(
            width(), share(*dst()), share(*src()), extensionMode());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitNot(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitNot(*this);
    }

protected:
    const char *mnemonic() const override {
        return "NOT";
    }
};

} // namespace mini_llvm::mir
