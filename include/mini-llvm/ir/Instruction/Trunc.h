#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/IntegerCastingOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class Trunc final : public IntegerCastingOperator {
public:
    Trunc(std::shared_ptr<Value> value, std::unique_ptr<IntegerType> type)
        : IntegerCastingOperator(std::move(value), std::move(type)) {}

    void accept(InstructionVisitor &visitor) override {
        visitor.visitTrunc(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitTrunc(*this);
    }

    std::unique_ptr<Constant> fold() const override;

    std::string format() const override {
        return std::format("{:o} = trunc {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<Trunc>(share(*value()), cast<IntegerType>(type()));
    }
};

} // namespace mini_llvm::ir
