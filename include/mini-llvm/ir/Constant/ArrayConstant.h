// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT ArrayConstant final : public Constant {
    using ElementList = std::vector<std::unique_ptr<Use<Constant>>>;

public:
    using element_iterator = IndirectIterator<ElementList::iterator, Use<Constant>>;
    using const_element_iterator = IndirectIterator<ElementList::const_iterator, const Use<Constant>>;

    ArrayConstant(std::unique_ptr<ArrayType> type, std::vector<std::shared_ptr<Constant>> elements);

    element_iterator element_begin() {
        return element_iterator(elements_.begin());
    }

    element_iterator element_end() {
        return element_iterator(elements_.end());
    }

    const_element_iterator element_begin() const {
        return const_element_iterator(elements_.begin());
    }

    const_element_iterator element_end() const {
        return const_element_iterator(elements_.end());
    }

    bool element_empty() const {
        return elements_.empty();
    }

    size_t element_size() const {
        return elements_.size();
    }

    Use<Constant> &element(size_t i) {
        return *elements_[i];
    }

    const Use<Constant> &element(size_t i) const {
        return *elements_[i];
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitArrayConstant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitArrayConstant(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

protected:
    bool equals(const Constant &other) const override;

private:
    std::unique_ptr<ArrayType> type_;
    ElementList elements_;
};

inline auto elements(ArrayConstant &C) {
    return std::ranges::subrange(C.element_begin(), C.element_end());
}

inline auto elements(const ArrayConstant &C) {
    return std::ranges::subrange(C.element_begin(), C.element_end());
}

} // namespace mini_llvm::ir
