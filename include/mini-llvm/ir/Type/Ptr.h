#pragma once

#include <memory>
#include <string>
#include <typeinfo>
#include <utility>

#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/TypeVisitor.h"

namespace mini_llvm::ir {

class Constant;

class Ptr final : public IntegerType {
public:
    int size() const override {
        std::unreachable();
    }

    int alignment() const override {
        std::unreachable();
    }

    int size(int pointerSizeAndAlignment) const override {
        return pointerSizeAndAlignment;
    }

    int alignment(int pointerSizeAndAlignment) const override {
        return pointerSizeAndAlignment;
    }

    std::unique_ptr<Constant> zeroValue() const override;

    std::string format() const override {
        return "ptr";
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<Ptr>();
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitPtr(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitPtr(*this);
    }

protected:
    bool equals(const Type &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
