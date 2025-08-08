// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/IntegerOrPointerType.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class Constant;

class MINI_LLVM_EXPORT Ptr final : public IntegerOrPointerType {
public:
    int size() const override {
        abort();
    }

    int alignment() const override {
        abort();
    }

    int size(int pointerSize) const override {
        return pointerSize;
    }

    int alignment(int pointerAlignment) const override {
        return pointerAlignment;
    }

    std::unique_ptr<Constant> zeroValue() const override;

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
