// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Double final : public FloatingType {
public:
    Precision precision() const override {
        return Precision::kDouble;
    }

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
        return "double";
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<Double>();
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitDouble(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitDouble(*this);
    }

protected:
    bool equals(const Type &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
