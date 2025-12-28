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

class MINI_LLVM_EXPORT UnaryFloatingOperator : public Instruction {
public:
    template <typename Self>
    auto &value(this Self &&self) {
        return self.value_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&value()};
    }

    bool isFoldable() const override;
    bool isWellFormed() const override;

protected:
    explicit UnaryFloatingOperator(std::shared_ptr<Value> value) : value_(this, std::move(value)) {}

private:
    Use<Value> value_;
};

} // namespace mini_llvm::ir
