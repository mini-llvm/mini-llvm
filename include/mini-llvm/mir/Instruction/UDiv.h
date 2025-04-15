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

class UDiv : public BinaryOperator {
public:
    UDiv(int width,
         std::shared_ptr<Register> dst,
         std::shared_ptr<Register> src1,
         std::shared_ptr<Register> src2,
         ExtensionMode extMode = ExtensionMode::kNo)
        : BinaryOperator(width, std::move(dst), std::move(src1), std::move(src2), extMode) {}

    bool hasSideEffects() const override {
        return true;
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<UDiv>(
            width(), share(*dst()), share(*src1()), share(*src2()), extMode());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitUDiv(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitUDiv(*this);
    }

protected:
    const char *mnemonic() const override {
        return "UDIV";
    }
};

} // namespace mini_llvm::mir
