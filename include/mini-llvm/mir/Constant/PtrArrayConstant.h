// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/GlobalValue.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT PtrArrayConstant final : public Constant {
public:
    PtrArrayConstant(int ptrSize, std::vector<std::pair<GlobalValue *, int64_t>> elements)
        : ptrSize_(ptrSize), elements_(std::move(elements)) {}

    int ptrSize() const {
        return ptrSize_;
    }

    std::vector<std::pair<GlobalValue *, int64_t>> &elements() & {
        return elements_;
    }

    const std::vector<std::pair<GlobalValue *, int64_t>> &elements() const & {
        return elements_;
    }

    std::vector<std::pair<GlobalValue *, int64_t>> &&elements() && {
        return std::move(elements_);
    }

    void setElements(std::vector<std::pair<GlobalValue *, int64_t>> elements) {
        elements_ = std::move(elements);
    }

    size_t numElements() const {
        return elements_.size();
    }

    int size() const override {
        return static_cast<int>(numElements()) * ptrSize_;
    }

    std::string format() const override;

    void accept(ConstantVisitor &visitor) override {
        visitor.visitPtrArrayConstant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitPtrArrayConstant(*this);
    }

private:
    int ptrSize_;
    std::vector<std::pair<GlobalValue *, int64_t>> elements_;
};

} // namespace mini_llvm::mir
