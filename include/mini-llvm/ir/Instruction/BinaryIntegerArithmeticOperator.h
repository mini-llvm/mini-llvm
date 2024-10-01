#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class BinaryIntegerArithmeticOperator : public BinaryIntegerOperator {
public:
    std::unique_ptr<Type> type() const override {
        return opType();
    }

    // x op y == y op x
    virtual bool isCommutative() const = 0;

    // (x op y) op z == x op (y op z)
    virtual bool isAssociative() const = 0;

protected:
    BinaryIntegerArithmeticOperator(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : BinaryIntegerOperator(std::move(lhs), std::move(rhs)) {}
};

} // namespace mini_llvm::ir
