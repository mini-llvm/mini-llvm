// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class I64Constant final : public IntegerConstant {
public:
    explicit I64Constant(int64_t value) : value_(value) {}

    int64_t value() const {
        return value_;
    }

    int64_t signExtendedValue() const override {
        return value();
    }

    int64_t zeroExtendedValue() const override {
        return value();
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitI64Constant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitI64Constant(*this);
    }

    std::unique_ptr<Type> type() const override {
        return std::make_unique<I64>();
    }

    std::string format() const override {
        return std::to_string(value());
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<I64Constant>(value());
    }

protected:
    bool equals(const Constant &other) const override {
        return typeid(*this) == typeid(other) && value() == static_cast<const I64Constant &>(other).value();
    }

private:
    int64_t value_;
};

} // namespace mini_llvm::ir
