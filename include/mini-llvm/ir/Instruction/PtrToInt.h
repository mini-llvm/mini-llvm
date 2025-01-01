#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class PtrToInt final : public Instruction {
public:
    PtrToInt(std::shared_ptr<Value> value, std::unique_ptr<IntegerType> type)
        : value_(this, std::move(value)), type_(std::move(type)) {}

    template <typename Self>
    auto &value(this Self &&self) {
        return self.value_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&value()};
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitPtrToInt(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitPtrToInt(*this);
    }

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override {
        return std::format("{:o} = ptrtoint {} {:o} to {}", *this, *value()->type(), *value(), *type());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<PtrToInt>(share(*value()), cast<IntegerType>(type()));
    }

private:
    Use<Value, Ptr> value_;
    std::unique_ptr<IntegerType> type_;
};

} // namespace mini_llvm::ir
