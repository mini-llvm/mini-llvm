// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/FUnaryOperator.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT FNeg : public FUnaryOperator {
public:
    FNeg(Precision precision, std::shared_ptr<Register> dst, std::shared_ptr<Register> src)
        : FUnaryOperator(precision, std::move(dst), std::move(src)) {}

    bool hasSideEffects() const override {
        return true;
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FNeg>(precision(), share(*dst()), share(*src()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFNeg(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFNeg(*this);
    }

protected:
    const char *mnemonic() const override {
        return "FNEG";
    }
};

} // namespace mini_llvm::mir
