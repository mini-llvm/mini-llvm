#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/FloatingCastingOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class FPExt final : public FloatingCastingOperator {
public:
    FPExt(std::shared_ptr<Value> value, std::unique_ptr<FloatingType> type)
        : FloatingCastingOperator(std::move(value), std::move(type)) {}

    std::unique_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFPExt(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFPExt(*this);
    }

    std::string format() const override {
        return std::format("{:o} = fpext {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<FPExt>(share(*value()), cast<FloatingType>(type()));
    }
};

} // namespace mini_llvm::ir
