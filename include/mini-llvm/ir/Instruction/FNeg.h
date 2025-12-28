// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/UnaryFloatingArithmeticOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FNeg final : public UnaryFloatingArithmeticOperator {
public:
    explicit FNeg(std::shared_ptr<Value> value) : UnaryFloatingArithmeticOperator(std::move(value)) {}

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFNeg(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFNeg(*this);
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;
};

} // namespace mini_llvm::ir
