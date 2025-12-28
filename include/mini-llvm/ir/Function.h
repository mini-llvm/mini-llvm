// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/Attribute.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/OptionalRef.h"

namespace mini_llvm::ir {

class BasicBlock;

class MINI_LLVM_EXPORT Function final : public GlobalValue {
    using ArgumentList = std::vector<std::shared_ptr<Argument>>;
    using AttributeList = std::vector<std::unique_ptr<Attribute>>;
    using BasicBlockList = std::list<std::shared_ptr<BasicBlock>>;

public:
    using arg_iterator = IndirectIterator<ArgumentList::iterator, Argument>;
    using const_arg_iterator = IndirectIterator<ArgumentList::const_iterator, const Argument>;

    using attr_iterator = IndirectIterator<AttributeList::const_iterator, Attribute>;
    using const_attr_iterator = IndirectIterator<AttributeList::const_iterator, const Attribute>;

    using iterator = IndirectIterator<BasicBlockList::iterator, BasicBlock>;
    using const_iterator = IndirectIterator<BasicBlockList::const_iterator, const BasicBlock>;
    using reverse_iterator = IndirectIterator<BasicBlockList::reverse_iterator, BasicBlock>;
    using const_reverse_iterator = IndirectIterator<BasicBlockList::const_reverse_iterator, const BasicBlock>;

    Function(std::unique_ptr<FunctionType> functionType, Linkage linkage);

    std::unique_ptr<FunctionType> functionType() const {
        return cast<FunctionType>(functionType_->clone());
    }

    Linkage linkage() const {
        return linkage_;
    }

    arg_iterator arg_begin() {
        return arg_iterator(args_.begin());
    }

    const_arg_iterator arg_begin() const {
        return const_arg_iterator(args_.begin());
    }

    arg_iterator arg_end() {
        return arg_iterator(args_.end());
    }

    const_arg_iterator arg_end() const {
        return const_arg_iterator(args_.end());
    }

    bool arg_empty() const {
        return args_.empty();
    }

    size_t arg_size() const {
        return args_.size();
    }

    Argument &arg(size_t i) {
        return *args_[i];
    }

    const Argument &arg(size_t i) const {
        return *args_[i];
    }

    attr_iterator attr_begin() const {
        return attr_iterator(attrs_.begin());
    }

    attr_iterator attr_end() const {
        return attr_iterator(attrs_.end());
    }

    bool attr_empty() const {
        return attrs_.empty();
    }

    size_t attr_size() const {
        return attrs_.size();
    }

    Attribute &attr(size_t i) {
        return *attrs_[i];
    }

    const Attribute &attr(size_t i) const {
        return *attrs_[i];
    }

    template <typename T>
        requires std::derived_from<T, Attribute>
    OptionalRef<Attribute> attr() {
        auto i = std::ranges::find_if(attrs_, [](const std::unique_ptr<Attribute> &attr) {
            return dynamic_cast<const T *>(&*attr);
        });
        if (i == attrs_.end()) {
            return std::nullopt;
        }
        return **i;
    }

    template <typename T>
    OptionalRef<const Attribute> attr() const {
        auto i = std::ranges::find_if(attrs_, [](const std::unique_ptr<Attribute> &attr) {
            return dynamic_cast<const T *>(&*attr);
        });
        if (i == attrs_.end()) {
            return std::nullopt;
        }
        return **i;
    }

    template <typename T>
        requires std::derived_from<T, Attribute>
    T &addAttr(std::unique_ptr<T> attr) {
        attrs_.push_back(std::move(attr));
        return static_cast<T &>(*attrs_.back());
    }

    template <typename T>
        requires std::derived_from<T, Attribute>
    void removeAttr() {
        std::erase_if(attrs_, [](const std::unique_ptr<Attribute> &attr) {
            return dynamic_cast<const T *>(&*attr);
        });
    }

    iterator begin() {
        return iterator(blocks_.begin());
    }

    const_iterator begin() const {
        return const_iterator(blocks_.begin());
    }

    iterator end() {
        return iterator(blocks_.end());
    }

    const_iterator end() const {
        return const_iterator(blocks_.end());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(blocks_.rbegin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(blocks_.rbegin());
    }

    reverse_iterator rend() {
        return reverse_iterator(blocks_.rend());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(blocks_.rend());
    }

    BasicBlock &front() {
        return *begin();
    }

    const BasicBlock &front() const {
        return *begin();
    }

    BasicBlock &back() {
        return *std::prev(end());
    }

    const BasicBlock &back() const {
        return *std::prev(end());
    }

    bool empty() const {
        return blocks_.empty();
    }

    size_t size() const {
        return blocks_.size();
    }

    BasicBlock &entry() {
        return front();
    }

    const BasicBlock &entry() const {
        return front();
    }

    BasicBlock &add(const_iterator pos, std::shared_ptr<BasicBlock> block);
    BasicBlock &add(const_iterator pos);
    BasicBlock &prepend(std::shared_ptr<BasicBlock> block);
    BasicBlock &prepend();
    BasicBlock &append(std::shared_ptr<BasicBlock> block);
    BasicBlock &append();

    void remove(const_iterator pos);
    void removeFirst();
    void removeLast();

    void clear();

    bool isDeclaration() const override {
        return empty();
    }

    bool isWellFormed() const override;

    void accept(ConstantVisitor &visitor) override {
        visitor.visitFunction(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitFunction(*this);
    }

    std::string format() const override;

protected:
    bool equals([[maybe_unused]] const Constant &other) const override {
        assert(typeid(*this) != typeid(other));
        return false;
    }

private:
    std::unique_ptr<FunctionType> functionType_;
    Linkage linkage_;
    ArgumentList args_;
    AttributeList attrs_;
    BasicBlockList blocks_;
};

inline auto args(Function &F) {
    return std::ranges::subrange(F.arg_begin(), F.arg_end());
}

inline auto args(const Function &F) {
    return std::ranges::subrange(F.arg_begin(), F.arg_end());
}

inline auto attrs(const Function &F) {
    return std::ranges::subrange(F.attr_begin(), F.attr_end());
}

MINI_LLVM_EXPORT std::string toDot(const Function &F);

} // namespace mini_llvm::ir
