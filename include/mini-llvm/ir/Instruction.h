#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class Instruction : public Value {
public:
    BasicBlock *parent() const {
        return parent_;
    }

    BasicBlock::iterator parentIterator() const {
        return parentIterator_.value();
    }

    virtual std::unordered_set<const UseBase *> operands() const = 0;
    std::unordered_set<UseBase *> operands();

    virtual bool isFoldable() const {
        return false;
    }

    virtual std::unique_ptr<Constant> fold() const {
        std::unreachable();
    }

    virtual void accept(InstructionVisitor &visitor) = 0;
    virtual void accept(InstructionVisitor &visitor) const = 0;

    std::string formatAsOperand() const override {
        return "%" + (!name().empty() ? name() : std::format("{:x}", reinterpret_cast<uintptr_t>(this)));
    }

private:
    mutable BasicBlock *parent_ = nullptr;
    mutable std::optional<BasicBlock::iterator> parentIterator_ = std::nullopt;

    friend class BasicBlock;
};

Instruction &addToParent(const Instruction &before, std::shared_ptr<Instruction> I);

template <typename T>
    requires std::derived_from<T, Instruction>
T &addToParent(const Instruction &before, std::shared_ptr<T> I) {
    return static_cast<T &>(addToParent(before, cast<Instruction>(std::move(I))));
}

void removeFromParent(const Instruction &I);

} // namespace mini_llvm::ir
