#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/BinaryOperator.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class SDiv : public BinaryOperator {
public:
    SDiv(int width,
         std::shared_ptr<Register> dst,
         std::shared_ptr<Register> src1,
         std::shared_ptr<Register> src2,
         ExtensionMode extensionMode = ExtensionMode::kNo)
        : BinaryOperator(width, std::move(dst), std::move(src1), std::move(src2), extensionMode) {}

    bool hasSideEffects() const override {
        return true;
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<SDiv>(
            width(), share(*dst()), share(*src1()), share(*src2()), extensionMode());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitSDiv(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitSDiv(*this);
    }

protected:
    const char *mnemonic() const override {
        return "SDIV";
    }
};

} // namespace mini_llvm::mir
