#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction/UnaryFloatingArithmeticOperator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class FNeg final : public UnaryFloatingArithmeticOperator {
public:
    explicit FNeg(std::shared_ptr<Value> value) : UnaryFloatingArithmeticOperator(std::move(value)) {}

    std::unique_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFNeg(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFNeg(*this);
    }

    std::string format() const override {
        return std::format("{:o} = fneg {} {:o}", *this, *value()->type(), *value());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<FNeg>(share(*value()));
    }
};

} // namespace mini_llvm::ir
