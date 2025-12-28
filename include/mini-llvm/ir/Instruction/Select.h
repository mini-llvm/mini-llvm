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

class MINI_LLVM_EXPORT Select final : public Instruction {
public:
    Select(std::shared_ptr<Value> cond, std::shared_ptr<Value> trueValue, std::shared_ptr<Value> falseValue)
        : cond_(this, std::move(cond)),
          trueValue_(this, std::move(trueValue)),
          falseValue_(this, std::move(falseValue)) {}

    template <typename Self>
    auto &cond(this Self &&self) {
        return self.cond_;
    }

    template <typename Self>
    auto &trueValue(this Self &&self) {
        return self.trueValue_;
    }

    template <typename Self>
    auto &falseValue(this Self &&self) {
        return self.falseValue_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&cond(), &trueValue(), &falseValue()};
    }

    bool isFoldable() const override;
    std::shared_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitSelect(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitSelect(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return trueValue()->type();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    Use<Value> cond_;
    Use<Value> trueValue_, falseValue_;
};

} // namespace mini_llvm::ir
