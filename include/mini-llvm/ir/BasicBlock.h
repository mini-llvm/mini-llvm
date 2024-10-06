#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/BasicBlockType.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/IndirectIterator.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class Instruction;

class BasicBlock final : public Value {
    using InstructionList = std::list<std::shared_ptr<Instruction>>;

public:
    using iterator = IndirectIterator<InstructionList::iterator, Instruction>;
    using const_iterator = IndirectIterator<InstructionList::const_iterator, const Instruction>;
    using reverse_iterator = IndirectIterator<InstructionList::reverse_iterator, Instruction>;
    using const_reverse_iterator = IndirectIterator<InstructionList::const_reverse_iterator, const Instruction>;

    iterator begin() {
        return iterator(insts_.begin());
    }

    const_iterator begin() const {
        return const_iterator(insts_.begin());
    }

    iterator end() {
        return iterator(insts_.end());
    }

    const_iterator end() const {
        return const_iterator(insts_.end());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(insts_.rbegin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(insts_.rbegin());
    }

    reverse_iterator rend() {
        return reverse_iterator(insts_.rend());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(insts_.rend());
    }

    Instruction &front() {
        return *begin();
    }

    const Instruction &front() const {
        return *begin();
    }

    Instruction &back() {
        return *std::prev(end());
    }

    const Instruction &back() const {
        return *std::prev(end());
    }

    bool empty() const {
        return insts_.empty();
    }

    size_t size() const {
        return insts_.size();
    }

    Instruction &add(const_iterator pos, std::shared_ptr<Instruction> I);

    Instruction &prepend(std::shared_ptr<Instruction> I) {
        return add(begin(), std::move(I));
    }

    Instruction &append(std::shared_ptr<Instruction> I) {
        return add(end(), std::move(I));
    }

    template <typename T>
        requires std::derived_from<T, Instruction>
    T &add(const_iterator pos, std::shared_ptr<T> I) {
        return static_cast<T &>(add(pos, cast<Instruction>(std::move(I))));
    }

    template <typename T>
        requires std::derived_from<T, Instruction>
    T &prepend(std::shared_ptr<T> I) {
        return static_cast<T &>(prepend(cast<Instruction>(std::move(I))));
    }

    template <typename T>
        requires std::derived_from<T, Instruction>
    T &append(std::shared_ptr<T> I) {
        return static_cast<T &>(append(cast<Instruction>(std::move(I))));
    }

    void remove(const_iterator pos);

    void removeFirst() {
        remove(begin());
    }

    void removeLast() {
        remove(std::prev(end()));
    }

    void clear() {
        while (!empty()) {
            removeFirst();
        }
    }

    Function *parent() const {
        return parent_;
    }

    Function::iterator parentIterator() const {
        return parentIterator_.value();
    }

    std::unique_ptr<Type> type() const override {
        return std::make_unique<BasicBlockType>();
    }

    std::string format() const override;

    std::string formatAsOperand() const override {
        return "%" + (!name().empty() ? name() : std::format("{:x}", reinterpret_cast<uintptr_t>(this)));
    }

    std::unique_ptr<Value> clone() const override {
        std::unreachable();
    }

private:
    InstructionList insts_;
    mutable Function *parent_ = nullptr;
    mutable std::optional<Function::iterator> parentIterator_ = std::nullopt;

    friend class Function;
};

bool hasNPredecessors(const BasicBlock &B, size_t n);
bool hasNPredecessorsOrMore(const BasicBlock &B, size_t n);
bool hasPredecessor(const BasicBlock &B, const BasicBlock &pred);
std::unordered_set<BasicBlock *> predecessors(const BasicBlock &B);
std::unordered_set<BasicBlock *> successors(const BasicBlock &B);
void removeFromParent(const BasicBlock &B);

} // namespace mini_llvm::ir
