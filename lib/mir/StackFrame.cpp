#include "mini-llvm/mir/StackFrame.h"

#include <memory>
#include <utility>

using namespace mini_llvm::mir;

StackSlot &StackFrame::add(StackFrame::const_iterator pos, std::unique_ptr<StackSlot> slot) {
    offsetsUpToDate_ = false;
    StackSlot &addedArea = **slots_.insert(pos.base(), std::move(slot));
    addedArea.frame_ = this;
    return addedArea;
}

std::unique_ptr<StackSlot> StackFrame::remove(StackFrame::iterator pos) {
    offsetsUpToDate_ = false;
    std::unique_ptr<StackSlot> removedArea = std::move(*pos.base());
    removedArea->frame_ = nullptr;
    slots_.erase(pos.base());
    return removedArea;
}

void StackFrame::computeOffsets() {
    if (offsetsUpToDate_ || slots_.empty())
        return;
    auto i = slots_.begin(), j = slots_.begin(), e = slots_.end();
    (*j)->offset_ = 0;
    ++j;
    for (; j != e; ++i, ++j)
        (*j)->offset_ =
            ((*i)->offset_ + (*i)->size() + (*j)->alignment() - 1) / (*j)->alignment() * (*j)->alignment();
    offsetsUpToDate_ = true;
}
