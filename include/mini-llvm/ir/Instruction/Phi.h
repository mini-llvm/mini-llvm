// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <iterator>
#include <list>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/ArrowProxy.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Phi final : public Instruction {
    using IncomingList = std::list<std::pair<std::unique_ptr<Use<BasicBlock>>, std::unique_ptr<Use<Value>>>>;

public:
    struct Incoming {
        Use<BasicBlock> &block;
        Use<Value> &value;

        Incoming(Use<BasicBlock> &block, Use<Value> &value) : block(block), value(value) {}
    };

    struct ConstIncoming {
        const Use<BasicBlock> &block;
        const Use<Value> &value;

        ConstIncoming(const Use<BasicBlock> &block, const Use<Value> &value) : block(block), value(value) {}
        ConstIncoming(const Incoming &incoming) : block(incoming.block), value(incoming.value) {}
    };

    class incoming_iterator {
    public:
        using iterator_category = std::iterator_traits<IncomingList::iterator>::iterator_category;
        using value_type = Incoming;
        using difference_type = std::iterator_traits<IncomingList::iterator>::difference_type;
        using pointer = ArrowProxy<Incoming>;
        using reference = Incoming;

        incoming_iterator() = default;

        ArrowProxy<Incoming> operator->() const {
            return ArrowProxy<Incoming>::make(*base_->first, *base_->second);
        }

        Incoming operator*() const {
            return Incoming(*base_->first, *base_->second);
        }

        incoming_iterator &operator++() {
            ++base_;
            return *this;
        }

        incoming_iterator operator++(int) {
            return incoming_iterator(base_++);
        }

        friend bool operator==(const incoming_iterator &lhs, const incoming_iterator &rhs) {
            return lhs.base_ == rhs.base_;
        }

        friend bool operator!=(const incoming_iterator &lhs, const incoming_iterator &rhs) {
            return lhs.base_ != rhs.base_;
        }

    private:
        IncomingList::iterator base_;

        explicit incoming_iterator(IncomingList::iterator base) : base_(base) {}

        friend class Phi;
    };

    class const_incoming_iterator {
    public:
        using iterator_category = std::iterator_traits<IncomingList::const_iterator>::iterator_category;
        using value_type = ConstIncoming;
        using difference_type = std::iterator_traits<IncomingList::const_iterator>::difference_type;
        using pointer = ArrowProxy<ConstIncoming>;
        using reference = ConstIncoming;

        const_incoming_iterator() = default;

        const_incoming_iterator(incoming_iterator i) : base_(i.base_) {}

        ArrowProxy<ConstIncoming> operator->() const {
            return ArrowProxy<ConstIncoming>::make(*base_->first, *base_->second);
        }

        ConstIncoming operator*() const {
            return ConstIncoming(*base_->first, *base_->second);
        }

        const_incoming_iterator &operator++() {
            ++base_;
            return *this;
        }

        const_incoming_iterator operator++(int) {
            return const_incoming_iterator(base_++);
        }

        friend bool operator==(const const_incoming_iterator &lhs, const const_incoming_iterator &rhs) {
            return lhs.base_ == rhs.base_;
        }

        friend bool operator!=(const const_incoming_iterator &lhs, const const_incoming_iterator &rhs) {
            return lhs.base_ != rhs.base_;
        }

    private:
        IncomingList::const_iterator base_;

        explicit const_incoming_iterator(IncomingList::const_iterator base) : base_(base) {}

        friend class Phi;
    };

    explicit Phi(std::unique_ptr<Type> type) : type_(std::move(type)) {}

    incoming_iterator incoming_begin() {
        return incoming_iterator(incomings_.begin());
    }

    incoming_iterator incoming_end() {
        return incoming_iterator(incomings_.end());
    }

    const_incoming_iterator incoming_begin() const {
        return const_incoming_iterator(incomings_.begin());
    }

    const_incoming_iterator incoming_end() const {
        return const_incoming_iterator(incomings_.end());
    }

    bool incoming_empty() const {
        return incomings_.empty();
    }

    size_t incoming_size() const {
        return incomings_.size();
    }

    incoming_iterator findIncoming(const BasicBlock &B);
    const_incoming_iterator findIncoming(const BasicBlock &B) const;
    void addIncoming(std::weak_ptr<BasicBlock> B, std::shared_ptr<Value> value);
    void removeIncoming(const_incoming_iterator i);

    std::unordered_set<const UseBase *> operands() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitPhi(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitPhi(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    std::unique_ptr<Type> type_;
    IncomingList incomings_;
};

inline auto incomings(Phi &I) {
    return std::ranges::subrange(I.incoming_begin(), I.incoming_end());
}

inline auto incomings(const Phi &I) {
    return std::ranges::subrange(I.incoming_begin(), I.incoming_end());
}

MINI_LLVM_EXPORT bool hasIncomingBlock(const Phi &I, const BasicBlock &B);
MINI_LLVM_EXPORT void removeIncomingBlock(Phi &I, const BasicBlock &B);
MINI_LLVM_EXPORT const Use<Value> &getIncomingValue(const Phi &I, const BasicBlock &B);
MINI_LLVM_EXPORT Use<Value> &getIncomingValue(Phi &I, const BasicBlock &B);
MINI_LLVM_EXPORT std::unordered_set<BasicBlock *> incomingBlocks(const Phi &I);
MINI_LLVM_EXPORT std::unordered_set<Value *> incomingValues(const Phi &I);

} // namespace mini_llvm::ir
