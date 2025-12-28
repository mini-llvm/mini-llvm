// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT BinaryFloatingOperator : public Instruction {
public:
    template <typename Self>
    auto &lhs(this Self &&self) {
        return self.lhs_;
    }

    template <typename Self>
    auto &rhs(this Self &&self) {
        return self.rhs_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&lhs(), &rhs()};
    }

    bool isFoldable() const override;
    bool isWellFormed() const override;

protected:
    BinaryFloatingOperator(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : lhs_(this, std::move(lhs)), rhs_(this, std::move(rhs)) {}

private:
    Use<Value> lhs_, rhs_;
};

} // namespace mini_llvm::ir
