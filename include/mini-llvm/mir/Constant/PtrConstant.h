// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/ConstantVisitor.h"
#include "mini-llvm/mir/GlobalValue.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT PtrConstant final : public Constant {
public:
    PtrConstant(int ptrSize, std::pair<GlobalValue *, int64_t> value)
        : ptrSize_(ptrSize), value_(value) {}

    PtrConstant(int ptrSize, GlobalValue *basePtr, int64_t offset)
        : ptrSize_(ptrSize), value_(basePtr, offset) {}

    int ptrSize() const {
        return ptrSize_;
    }

    std::pair<GlobalValue *, int64_t> value() const {
        return value_;
    }

    void setValue(std::pair<GlobalValue *, int64_t> value) {
        value_ = value;
    }

    void setValue(GlobalValue *basePtr, int64_t offset) {
        value_ = {basePtr, offset};
    }

    int size() const override {
        return ptrSize_;
    }

    std::string format() const override;

    void accept(ConstantVisitor &visitor) override {
        visitor.visitPtrConstant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitPtrConstant(*this);
    }

private:
    int ptrSize_;
    std::pair<GlobalValue *, int64_t> value_;
};

} // namespace mini_llvm::mir
