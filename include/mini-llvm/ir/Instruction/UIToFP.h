#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/IntegerToFloatingCastingOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class UIToFP final : public IntegerToFloatingCastingOperator {
public:
    UIToFP(std::shared_ptr<Value> value, std::unique_ptr<FloatingType> type)
        : IntegerToFloatingCastingOperator(std::move(value), std::move(type)) {}

    std::unique_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitUIToFP(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitUIToFP(*this);
    }

    std::string format() const override {
        return std::format("{:o} = uitofp {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<UIToFP>(share(*value()), cast<FloatingType>(type()));
    }
};

} // namespace mini_llvm::ir
