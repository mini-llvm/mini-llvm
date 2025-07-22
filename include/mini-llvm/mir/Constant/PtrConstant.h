#pragma once

#include <string>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/ConstantVisitor.h"
#include "mini-llvm/mir/GlobalValue.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT PtrConstant final : public Constant {
public:
    PtrConstant(int ptrSize, GlobalValue *ptr)
        : ptrSize_(ptrSize), ptr_(ptr) {}

    int ptrSize() const {
        return ptrSize_;
    }

    GlobalValue *ptr() const {
        return ptr_;
    }

    void setPtr(GlobalValue *ptr) {
        ptr_ = ptr;
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
    GlobalValue *ptr_;
};

} // namespace mini_llvm::mir
