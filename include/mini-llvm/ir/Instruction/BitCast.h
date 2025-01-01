#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/FloatingConstant.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class BitCast final : public Instruction {
public:
    BitCast(std::shared_ptr<Value> value, std::unique_ptr<Type> type)
        : value_(this, std::move(value)), type_(std::move(type)) {}

    template <typename Self>
    auto &value(this Self &&self) {
        return self.value_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&value()};
    }

    bool isFoldable() const override {
        return ((dynamic_cast<const IntegerConstant *>(&*value()) || dynamic_cast<const FloatingConstant *>(&*value()))
                    && (dynamic_cast<const IntegerType *>(&*type()) || dynamic_cast<const FloatingType *>(&*type()))
                    && *type() != Ptr())
            || dynamic_cast<const PoisonValue *>(&*value());
    }

    std::unique_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitBitCast(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitBitCast(*this);
    }

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override {
        return std::format("{:o} = bitcast {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<BitCast>(share(*value()), type());
    }

private:
    Use<Value> value_;
    std::unique_ptr<Type> type_;
};

} // namespace mini_llvm::ir
