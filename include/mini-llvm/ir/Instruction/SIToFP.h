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

class SIToFP final : public IntegerToFloatingCastingOperator {
public:
    SIToFP(std::shared_ptr<Value> value, std::unique_ptr<FloatingType> type)
        : IntegerToFloatingCastingOperator(std::move(value), std::move(type)) {}

    std::unique_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitSIToFP(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitSIToFP(*this);
    }

    std::string format() const override {
        return std::format("{:o} = sitofp {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<SIToFP>(share(*value()), cast<FloatingType>(type()));
    }
};

} // namespace mini_llvm::ir
