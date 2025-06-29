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

class ZExt final : public IntegerCastingOperator {
public:
    ZExt(std::shared_ptr<Value> value, std::unique_ptr<IntegerType> type)
        : IntegerCastingOperator(std::move(value), std::move(type)) {}

    void accept(InstructionVisitor &visitor) override {
        visitor.visitZExt(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitZExt(*this);
    }

    std::shared_ptr<Constant> fold() const override;

    std::string format() const override {
        return std::format("{:o} = zext {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<ZExt>(share(*value()), cast<IntegerType>(type()));
    }
};

} // namespace mini_llvm::ir
