// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FunctionType final : public Type {
public:
    using param_type_iterator = IndirectIterator<std::vector<std::unique_ptr<Type>>::const_iterator, Type>;

    FunctionType(std::unique_ptr<Type> returnType, std::vector<std::unique_ptr<Type>> paramTypes, bool isVarArgs)
        : returnType_(std::move(returnType)), paramTypes_(std::move(paramTypes)), isVarArgs_(isVarArgs) {}

    std::unique_ptr<Type> returnType() const {
        return returnType_->clone();
    }

    param_type_iterator param_type_begin() const {
        return param_type_iterator(paramTypes_.begin());
    }

    param_type_iterator param_type_end() const {
        return param_type_iterator(paramTypes_.end());
    }

    bool param_type_empty() const {
        return paramTypes_.empty();
    }

    size_t param_type_size() const {
        return paramTypes_.size();
    }

    bool isVarArgs() const {
        return isVarArgs_;
    }

    int size() const override {
        abort();
    }

    int alignment() const override {
        abort();
    }

    std::unique_ptr<Constant> zeroValue() const override {
        abort();
    }

    std::unique_ptr<Constant> constant(int64_t) const override {
        abort();
    }

    std::unique_ptr<Type> promoted() const override {
        abort();
    }

    std::unique_ptr<Type> demoted() const override {
        abort();
    }

    std::string format() const override;
    std::unique_ptr<Type> clone() const override;

    void accept(TypeVisitor &visitor) override {
        visitor.visitFunctionType(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitFunctionType(*this);
    }

protected:
    bool equals(const Type &other) const override;

private:
    std::unique_ptr<Type> returnType_;
    std::vector<std::unique_ptr<Type>> paramTypes_;
    bool isVarArgs_;
};

inline auto paramTypes(const FunctionType &type) {
    return std::ranges::subrange(type.param_type_begin(), type.param_type_end());
}

} // namespace mini_llvm::ir
