// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Type.h"

namespace mini_llvm::ir {

class PoisonValue final : public Constant {
public:
    explicit PoisonValue(std::unique_ptr<Type> type) : type_(std::move(type)) {}

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitPoisonValue(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitPoisonValue(*this);
    }

    std::string format() const override {
        return "poison";
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<PoisonValue>(type());
    }

protected:
    bool equals(const Constant &other) const override {
        return typeid(*this) == typeid(other);
    }

private:
    std::unique_ptr<Type> type_;
};

} // namespace mini_llvm::ir
