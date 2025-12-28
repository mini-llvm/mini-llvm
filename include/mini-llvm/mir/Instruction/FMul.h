// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/FBinaryOperator.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT FMul : public FBinaryOperator {
public:
    FMul(Precision precision,
         std::shared_ptr<Register> dst,
         std::shared_ptr<Register> src1,
         std::shared_ptr<Register> src2)
        : FBinaryOperator(precision, std::move(dst), std::move(src1), std::move(src2)) {}

    bool hasSideEffects() const override {
        return true;
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FMul>(
            precision(), share(*dst()), share(*src1()), share(*src2()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFMul(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFMul(*this);
    }

protected:
    const char *mnemonic() const override {
        return "FMUL";
    }
};

} // namespace mini_llvm::mir
