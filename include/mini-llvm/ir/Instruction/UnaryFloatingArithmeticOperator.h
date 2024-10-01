#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/ir/Instruction/UnaryFloatingOperator.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class UnaryFloatingArithmeticOperator : public UnaryFloatingOperator {
public:
    std::unique_ptr<Type> type() const override {
        return value()->type();
    }

protected:
    explicit UnaryFloatingArithmeticOperator(std::shared_ptr<Value> op)
        : UnaryFloatingOperator(std::move(op)) {}
};

} // namespace mini_llvm::ir
