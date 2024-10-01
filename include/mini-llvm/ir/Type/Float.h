#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FloatingType.h"
#include "mini-llvm/ir/TypeVisitor.h"

namespace mini_llvm::ir {

class Float final : public FloatingType {
public:
    Precision precision() const override {
        return Precision::kSingle;
    }

    int size() const override {
        return 4;
    }

    int alignment() const override {
        return 4;
    }

    std::unique_ptr<Constant> zeroValue() const override;

    std::string format() const override {
        return "float";
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<Float>();
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitFloat(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitFloat(*this);
    }

protected:
    bool equals(const Type &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
