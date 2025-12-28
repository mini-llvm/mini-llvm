// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <cstdlib>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Instruction : public Value {
public:
    BasicBlock *parent() const {
        return parent_;
    }

    BasicBlock::iterator parentIterator() const {
        return *parentIterator_;
    }

    virtual std::unordered_set<const UseBase *> operands() const = 0;
    std::unordered_set<UseBase *> operands();

    virtual bool isFoldable() const {
        return false;
    }

    virtual std::shared_ptr<Constant> fold() const {
        abort();
    }

    virtual void accept(InstructionVisitor &visitor) = 0;
    virtual void accept(InstructionVisitor &visitor) const = 0;

    bool isWellFormed() const override;

    std::string formatAsOperand() const override {
        return "%" + formatName();
    }

private:
    mutable BasicBlock *parent_ = nullptr;
    mutable std::optional<BasicBlock::iterator> parentIterator_ = std::nullopt;

    friend class BasicBlock;
};

MINI_LLVM_EXPORT Instruction &addToParent(const Instruction &before, std::shared_ptr<Instruction> I);

template <typename T>
    requires std::derived_from<T, Instruction>
T &addToParent(const Instruction &before, std::shared_ptr<T> I) {
    return static_cast<T &>(addToParent(before, cast<Instruction>(std::move(I))));
}

MINI_LLVM_EXPORT void removeFromParent(const Instruction &I);

} // namespace mini_llvm::ir
