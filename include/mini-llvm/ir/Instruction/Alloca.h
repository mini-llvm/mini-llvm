#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class Alloca final : public Instruction {
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

    std::unique_ptr<Type> type() const override {
        return std::make_unique<Ptr>();
    }

    std::string format() const override {
        return std::format("{:o} = alloca {}", *this, *allocatedType());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<Alloca>(allocatedType());
    }

private:
    std::unique_ptr<Type> allocatedType_;
};

} // namespace mini_llvm::ir
