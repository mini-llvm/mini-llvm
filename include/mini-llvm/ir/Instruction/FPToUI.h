// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/FloatingToIntegerCastingOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FPToUI final : public FloatingToIntegerCastingOperator {
public:
    FPToUI(std::shared_ptr<Value> value, std::unique_ptr<IntegerType> type)
        : FloatingToIntegerCastingOperator(std::move(value), std::move(type)) {}

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFPToUI(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFPToUI(*this);
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;
};

} // namespace mini_llvm::ir
