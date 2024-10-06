#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/FloatingToIntegerCastingOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class FPToSI final : public FloatingToIntegerCastingOperator {
public:
    FPToSI(std::shared_ptr<Value> value, std::unique_ptr<IntegerType> type)
        : FloatingToIntegerCastingOperator(std::move(value), std::move(type)) {}

    std::unique_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFPToSI(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFPToSI(*this);
    }

    std::string format() const override {
        return std::format("{:o} = fptosi {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<FPToSI>(share(*value()), cast<IntegerType>(type()));
    }
};

} // namespace mini_llvm::ir
