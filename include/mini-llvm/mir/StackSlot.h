// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class StackFrame;

class MINI_LLVM_EXPORT StackSlot {
public:
    StackSlot(int size, int alignment) : size_(size), alignment_(alignment) {}
    StackSlot(const StackSlot &) = delete;
    StackSlot(StackSlot &&) = delete;
    StackSlot &operator=(const StackSlot &) = delete;
    StackSlot &operator=(StackSlot &&) = delete;

    int size() const {
        return size_;
    }

    int alignment() const {
        return alignment_;
    }

    StackFrame *frame() const {
        return frame_;
    }

    int offset() const;

private:
    int size_, alignment_;
    mutable StackFrame *frame_{};
    mutable int offset_{};

    friend class StackFrame;
};

} // namespace mini_llvm::mir
