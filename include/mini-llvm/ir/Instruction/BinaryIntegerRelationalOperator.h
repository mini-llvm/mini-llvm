// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT BinaryIntegerRelationalOperator : public BinaryIntegerOperator {
public:
    std::unique_ptr<Type> type() const override {
        return std::make_unique<I1>();
    }

protected:
    BinaryIntegerRelationalOperator(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : BinaryIntegerOperator(std::move(lhs), std::move(rhs)) {}
};

} // namespace mini_llvm::ir
