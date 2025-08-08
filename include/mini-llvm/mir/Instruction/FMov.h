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

class MINI_LLVM_EXPORT FMov : public FUnaryOperator {
public:
    FMov(Precision precision, std::shared_ptr<Register> dst, std::shared_ptr<Register> src)
        : FUnaryOperator(precision, std::move(dst), std::move(src)) {}

    bool isCoalescent() const override {
        return true;
    }

    Register *coalescenceDst() const override {
        return &*dst();
    }

    Register *coalescenceSrc() const override {
        return &*src();
    }

    bool hasSideEffects() const override {
        return false;
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FMov>(precision(), share(*dst()), share(*src()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFMov(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFMov(*this);
    }

protected:
    const char *mnemonic() const override {
        return "FMOV";
    }
};

} // namespace mini_llvm::mir
