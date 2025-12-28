// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Alloca final : public Instruction {
public:
    explicit Alloca(std::unique_ptr<Type> allocatedType) : allocatedType_(std::move(allocatedType)) {}

    std::unique_ptr<Type> allocatedType() const {
        return allocatedType_->clone();
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {};
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitAlloca(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitAlloca(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return std::make_unique<Ptr>();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    std::unique_ptr<Type> allocatedType_;
};

} // namespace mini_llvm::ir
