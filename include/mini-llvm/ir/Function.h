#pragma once

#include <algorithm>
#include <cstddef>
#include <list>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/Attribute.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/utils/IndirectIterator.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class BasicBlock;

class Function final : public GlobalValue {
    using ArgumentList = std::vector<std::shared_ptr<Argument>>;
    using AttributeList = std::vector<Attribute>;
    using BasicBlockList = std::list<std::shared_ptr<BasicBlock>>;

public:
    using arg_iterator = IndirectIterator<ArgumentList::iterator, Argument>;
    using const_arg_iterator = IndirectIterator<ArgumentList::const_iterator, const Argument>;

    using attr_iterator = AttributeList::const_iterator;

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

    attr_iterator attr_begin() const {
        return attrs_.begin();
    }

    attr_iterator attr_end() const {
        return attrs_.end();
    }

    bool attr_empty() const {
        return attrs_.empty();
    }

    size_t attr_size() const {
        return attrs_.size();
    }

    bool hasAttr(Attribute attr) const {
        return std::ranges::contains(attrs_, attr);
    }

    void setAttr(Attribute attr) {
        if (!std::ranges::contains(attrs_, attr)) {
            attrs_.push_back(attr);
        }
    }

    void clearAttr(Attribute attr) {
        std::erase(attrs_, attr);
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

    void accept(ConstantVisitor &visitor) override {
        visitor.visitFunction(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitFunction(*this);
    }

    std::string format() const override;

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

} // namespace mini_llvm::ir
