#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/TypeVisitor.h"

namespace mini_llvm::ir {

class I32 final : public IntegerType {
public:
    int size() const override {
        return 4;
    }

    int alignment() const override {
        return 4;
    }

    std::unique_ptr<Constant> zeroValue() const override;

    std::string format() const override {
        return "i32";
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<I32>();
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitI32(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitI32(*this);
    }

protected:
    bool equals(const Type &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
