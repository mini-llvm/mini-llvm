#pragma once

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingArithmeticOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class FRem final : public BinaryFloatingArithmeticOperator {
public:
    FRem(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : BinaryFloatingArithmeticOperator(std::move(lhs), std::move(rhs)) {}

    bool isCommutative() const override {
        return false;
    }

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFRem(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFRem(*this);
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;
};

} // namespace mini_llvm::ir
