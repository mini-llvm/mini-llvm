// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT LSHR final : public BinaryIntegerArithmeticOperator {
public:
    LSHR(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : BinaryIntegerArithmeticOperator(std::move(lhs), std::move(rhs)) {}

    bool isCommutative() const override {
        return false;
    }

    bool isAssociative() const override {
        return false;
    }

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLSHR(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLSHR(*this);
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;
};

} // namespace mini_llvm::ir
