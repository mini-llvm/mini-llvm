#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/BinaryFloatingArithmeticOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class FDiv final : public BinaryFloatingArithmeticOperator {
public:
    FDiv(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : BinaryFloatingArithmeticOperator(std::move(lhs), std::move(rhs)) {}

    bool isCommutative() const override {
        return false;
    }

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFDiv(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFDiv(*this);
    }

    std::string format() const override {
        return std::format("{:o} = fdiv {} {:o}, {:o}", *this, *lhs()->type(), *lhs(), *rhs());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<FDiv>(share(*lhs()), share(*rhs()));
    }
};

} // namespace mini_llvm::ir
