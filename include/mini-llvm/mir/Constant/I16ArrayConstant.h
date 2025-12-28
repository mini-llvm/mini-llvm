// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/ConstantVisitor.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT I16ArrayConstant final : public Constant {
public:
    explicit I16ArrayConstant(std::vector<int16_t> elements) : elements_(std::move(elements)) {}

    std::vector<int16_t> &elements() & {
        return elements_;
    }

    const std::vector<int16_t> &elements() const & {
        return elements_;
    }

    std::vector<int16_t> &&elements() && {
        return std::move(elements_);
    }

    void setElements(std::vector<int16_t> elements) {
        elements_ = std::move(elements);
    }

    size_t numElements() const {
        return elements_.size();
    }

    int size() const override {
        return static_cast<int>(numElements()) * 2;
    }

    std::string format() const override;

    void accept(ConstantVisitor &visitor) override {
        visitor.visitI16ArrayConstant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitI16ArrayConstant(*this);
    }

private:
    std::vector<int16_t> elements_;
};

} // namespace mini_llvm::mir
