#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class URem final : public BinaryIntegerArithmeticOperator {
public:
    URem(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : BinaryIntegerArithmeticOperator(std::move(lhs), std::move(rhs)) {}

    bool isCommutative() const override {
        return false;
    }

    bool isAssociative() const override {
        return false;
    }

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitURem(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitURem(*this);
    }

    std::string format() const override {
        return std::format("{:o} = urem {} {:o}, {:o}", *this, *lhs()->type(), *lhs(), *rhs());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<URem>(share(*lhs()), share(*rhs()));
    }
};

} // namespace mini_llvm::ir
