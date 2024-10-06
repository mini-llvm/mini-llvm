#pragma once

#include <cassert>
#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class Select final : public Instruction {
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

    std::unique_ptr<Type> opType() const {
        assert(*trueValue()->type() == *falseValue()->type());
        return trueValue()->type();
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&cond(), &trueValue(), &falseValue()};
    }

    bool isFoldable() const override {
        return dynamic_cast<const Constant *>(&*cond()) &&
            dynamic_cast<const Constant *>(&*trueValue()) &&
            dynamic_cast<const Constant *>(&*falseValue());
    }

    std::unique_ptr<Constant> fold() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitSelect(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitSelect(*this);
    }

    std::unique_ptr<Type> type() const override {
        return opType();
    }

    std::string format() const override {
        return std::format(
            "{:o} = select {} {:o}, {} {:o}, {} {:o}",
            *this, *cond()->type(), *cond(), *trueValue()->type(), *trueValue(), *falseValue()->type(), *falseValue());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<Select>(share(*cond()), share(*trueValue()), share(*falseValue()));
    }

private:
    Use<Value, I1> cond_;
    Use<Value> trueValue_, falseValue_;
};

} // namespace mini_llvm::ir
