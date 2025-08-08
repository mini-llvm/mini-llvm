// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT I8 final : public IntegerType {
public:
    int size() const override {
        return 1;
    }

    int alignment() const override {
        return 1;
    }

    std::unique_ptr<Constant> zeroValue() const override;
    std::unique_ptr<Constant> constant(int64_t value) const override;

    std::unique_ptr<Type> promoted() const override;

    std::unique_ptr<Type> demoted() const override {
        abort();
    }

    std::string format() const override {
        return "i8";
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<I8>();
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitI8(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitI8(*this);
    }

protected:
    bool equals(const Type &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
