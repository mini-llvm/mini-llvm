// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT BitCast final : public Instruction {
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

    bool isFoldable() const override;

    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitBitCast(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitBitCast(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    Use<Value> value_;
    std::unique_ptr<Type> type_;
};

} // namespace mini_llvm::ir
