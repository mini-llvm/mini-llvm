// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class VoidValue final : public Constant {
public:
    void accept(ConstantVisitor &visitor) override {
        visitor.visitVoidValue(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitVoidValue(*this);
    }

    std::unique_ptr<Type> type() const override {
        return std::make_unique<Void>();
    }

    std::string format() const override {
        abort();
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<VoidValue>();
    }

protected:
    bool equals(const Constant &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
