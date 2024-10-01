#pragma once

#include <cstddef>
#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/TypeVisitor.h"

namespace mini_llvm::ir {

class ArrayType final : public Type {
public:
    ArrayType(std::unique_ptr<Type> elementType, size_t numElements)
        : elementType_(std::move(elementType)), numElements_(numElements) {}

    std::unique_ptr<Type> elementType() const {
        return elementType_->clone();
    }

    size_t numElements() const {
        return numElements_;
    }

    int size() const override {
        return elementType()->size() * static_cast<int>(numElements());
    }

    int alignment() const override {
        return elementType()->alignment();
    }

    int size(int pointerSizeAndAlignment) const override {
        return elementType()->size(pointerSizeAndAlignment) * static_cast<int>(numElements());
    }

    int alignment(int pointerSizeAndAlignment) const override {
        return elementType()->alignment(pointerSizeAndAlignment);
    }

    std::unique_ptr<Constant> zeroValue() const override;

    std::string format() const override {
        return std::format("[{} x {}]", numElements(), *elementType());
    }

    std::unique_ptr<Type> clone() const override {
        return std::make_unique<ArrayType>(elementType(), numElements());
    }

    void accept(TypeVisitor &visitor) override {
        visitor.visitArrayType(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitArrayType(*this);
    }

protected:
    bool equals(const Type &other) const override;

private:
    std::unique_ptr<Type> elementType_;
    size_t numElements_;
};

} // namespace mini_llvm::ir
