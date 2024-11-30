#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Panic.h"

namespace mini_llvm::ir {

class BasicBlockType final : public Type {
public:
    int size() const override {
        panic();
    }

    int alignment() const override {
        panic();
    }

    std::unique_ptr<Constant> zeroValue() const override {
        panic();
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
