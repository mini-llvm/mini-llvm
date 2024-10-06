#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class NullPtrConstant final : public IntegerConstant {
public:
    int64_t signExtendedValue() const override {
        return 0;
    }

    int64_t zeroExtendedValue() const override {
        return 0;
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitNullPtrConstant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitNullPtrConstant(*this);
    }

    std::unique_ptr<Type> type() const override {
        return std::make_unique<Ptr>();
    }

    std::string format() const override {
        return "null";
    }

    std::unique_ptr<Value> clone() const override {
        return std::make_unique<NullPtrConstant>();
    }

protected:
    bool equals(const Constant &other) const override {
        return typeid(*this) == typeid(other);
    }
};

} // namespace mini_llvm::ir
