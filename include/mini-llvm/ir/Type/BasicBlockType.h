// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/TypeVisitor.h"

namespace mini_llvm::ir {

class BasicBlockType final : public Type {
public:
    int size() const override {
        abort();
    }

    int alignment() const override {
        abort();
    }

    std::unique_ptr<Constant> zeroValue() const override {
        abort();
    }

    std::unique_ptr<Constant> constant(int64_t) const override {
        abort();
    }

    std::unique_ptr<Type> promoted() const override {
        abort();
    }

    std::unique_ptr<Type> demoted() const override {
        abort();
    }

    std::string format() const override {
        return "label";
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<BasicBlockType>();
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitBasicBlockType(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitBasicBlockType(*this);
    }

protected:
    bool equals(const Type &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
