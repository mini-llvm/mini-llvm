// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Load final : public Instruction {
public:
    Load(std::unique_ptr<Type> type, std::shared_ptr<Value> addr)
        : type_(std::move(type)), ptr_(this, std::move(addr)) {}

    template <typename Self>
    auto &ptr(this Self &&self) {
        return self.ptr_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&ptr()};
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLoad(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLoad(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    std::unique_ptr<Type> type_;
    Use<Value> ptr_;
};

} // namespace mini_llvm::ir
