// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/common/ops/SExt.h"
#include "mini-llvm/common/ops/ZExt.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class I1Constant final : public IntegerConstant {
public:
    explicit I1Constant(bool value) : value_(value) {}

    bool value() const {
        return value_;
    }

    int64_t signExtendedValue() const override {
        return ops::SExt<int64_t>()(value());
    }

    int64_t zeroExtendedValue() const override {
        return ops::ZExt<int64_t>()(value());
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitI1Constant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitI1Constant(*this);
    }

    std::unique_ptr<Type> type() const override {
        return std::make_unique<I1>();
    }

    std::string format() const override {
        return value() ? "true" : "false";
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<I1Constant>(value());
    }

protected:
    bool equals(const Constant &other) const override {
        return typeid(*this) == typeid(other) && value() == static_cast<const I1Constant &>(other).value();
    }

private:
    bool value_;
};

} // namespace mini_llvm::ir
