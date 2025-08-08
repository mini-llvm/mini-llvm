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

class MINI_LLVM_EXPORT I64 final : public IntegerType {
public:
    int size() const override {
        return 8;
    }

    int alignment() const override {
        return 8;
    }

    std::unique_ptr<Constant> zeroValue() const override;
    std::unique_ptr<Constant> constant(int64_t value) const override;

    std::unique_ptr<Type> promoted() const override {
        abort();
    }

    std::unique_ptr<Type> demoted() const override;

    std::string format() const override {
        return "i64";
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<I64>();
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitI64(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitI64(*this);
    }

protected:
    bool equals(const Type &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
