#pragma once

#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class UnaryFloatingOperator : public Instruction {
public:
    template <typename Self>
    auto &value(this Self &&self) {
        return self.value_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&value()};
    }

    bool isFoldable() const override {
        return dynamic_cast<const Constant *>(&*value());
    }

protected:
    explicit UnaryFloatingOperator(std::shared_ptr<Value> value) : value_(this, std::move(value)) {}

private:
    Use<Value, FloatingType> value_;
};

} // namespace mini_llvm::ir
