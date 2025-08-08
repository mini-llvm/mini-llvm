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
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class I16Constant final : public IntegerConstant {
public:
    explicit I16Constant(int16_t value) : value_(value) {}

    int16_t value() const {
        return value_;
    }

    int64_t signExtendedValue() const override {
        return ops::SExt<int64_t>()(value());
    }

    int64_t zeroExtendedValue() const override {
        return ops::ZExt<int64_t>()(value());
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitI16Constant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitI16Constant(*this);
    }

    std::unique_ptr<Type> type() const override {
        return std::make_unique<I16>();
    }

    std::string format() const override {
        return std::to_string(value());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<I16Constant>(value());
    }

protected:
    bool equals(const Constant &other) const override {
        return typeid(*this) == typeid(other) && value() == static_cast<const I16Constant &>(other).value();
    }

private:
    int16_t value_;
};

} // namespace mini_llvm::ir
