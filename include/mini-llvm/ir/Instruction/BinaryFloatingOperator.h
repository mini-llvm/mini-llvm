#pragma once

#include <cassert>
#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class BinaryFloatingOperator : public Instruction {
public:
    template <typename Self>
    auto &lhs(this Self &&self) {
        return self.lhs_;
    }

    template <typename Self>
    auto &rhs(this Self &&self) {
        return self.rhs_;
    }

    std::unique_ptr<Type> opType() const {
        assert(*lhs()->type() == *rhs()->type());
        return lhs()->type();
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&lhs(), &rhs()};
    }

    bool isFoldable() const override {
        return dynamic_cast<const Constant *>(&*lhs()) && dynamic_cast<const Constant *>(&*rhs());
    }

protected:
    BinaryFloatingOperator(std::shared_ptr<Value> lhs, std::shared_ptr<Value> rhs)
        : lhs_(this, std::move(lhs)), rhs_(this, std::move(rhs)) {}

private:
    Use<Value, FloatingType> lhs_, rhs_;
};

} // namespace mini_llvm::ir
