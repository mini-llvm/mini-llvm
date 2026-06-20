// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/StackSlot.h"

#include <cassert>

#include "mini-llvm/mir/StackFrame.h"

namespace mini_llvm::mir {

int StackSlot::offset() const {
    assert(frame_ != nullptr);
    frame_->computeOffsets();
    return offset_;
}

} // namespace mini_llvm::mir
