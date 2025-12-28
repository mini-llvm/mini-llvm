// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <iterator>
#include <list>
#include <memory>
#include <utility>

#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT StackFrame {
    using StackSlotList =  std::list<std::unique_ptr<StackSlot>>;

public:
    using iterator = IndirectIterator<StackSlotList::iterator, StackSlot>;
    using const_iterator = IndirectIterator<StackSlotList::const_iterator, const StackSlot>;
    using reverse_iterator = IndirectIterator<StackSlotList::reverse_iterator, StackSlot>;
    using const_reverse_iterator = IndirectIterator<StackSlotList::const_reverse_iterator, const StackSlot>;

    StackFrame() = default;
    StackFrame(const StackFrame &) = delete;
    StackFrame(StackFrame &&) = delete;
    StackFrame &operator=(const StackFrame &) = delete;
    StackFrame &operator=(StackFrame &&) = delete;

    iterator begin() {
        return iterator(slots_.begin());
    }

    const_iterator begin() const {
        return const_iterator(slots_.begin());
    }

    iterator end() {
        return iterator(slots_.end());
    }

    const_iterator end() const {
        return const_iterator(slots_.end());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(slots_.rbegin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(slots_.rbegin());
    }

    reverse_iterator rend() {
        return reverse_iterator(slots_.rend());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(slots_.rend());
    }

    StackSlot &front() {
        return *begin();
    }

    const StackSlot &front() const {
        return *begin();
    }

    StackSlot &back() {
        return *std::prev(end());
    }

    const StackSlot &back() const {
        return *std::prev(end());
    }

    bool empty() const {
        return slots_.empty();
    }

    size_t size() const {
        return slots_.size();
    }

    StackSlot &add(const_iterator pos, std::unique_ptr<StackSlot> slot);

    StackSlot &add(const_iterator pos, int size, int alignment) {
        return add(pos, std::make_unique<StackSlot>(size, alignment));
    }

    StackSlot &prepend(std::unique_ptr<StackSlot> slot) {
        return add(begin(), std::move(slot));
    }

    StackSlot &prepend(int size, int alignment) {
        return prepend(std::make_unique<StackSlot>(size, alignment));
    }

    StackSlot &append(std::unique_ptr<StackSlot> slot) {
        return add(end(), std::move(slot));
    }

    StackSlot &append(int size, int alignment) {
        return append(std::make_unique<StackSlot>(size, alignment));
    }

    std::unique_ptr<StackSlot> remove(iterator pos);

    std::unique_ptr<StackSlot> removeFirst() {
        return remove(begin());
    }

    std::unique_ptr<StackSlot> removeLast() {
        return remove(std::prev(end()));
    }

    void clear();

private:
    StackSlotList slots_;
    bool offsetsUpToDate_ = false;

    void computeOffsets();

    friend class StackSlot;
};

} // namespace mini_llvm::mir
