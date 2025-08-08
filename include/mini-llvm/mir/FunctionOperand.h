// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/Function.h"

namespace mini_llvm::mir {

class FunctionOperand {
public:
    explicit FunctionOperand(Function *function) : function_(function) {}

    Function &operator*() const {
        return *function_;
    }

    Function *operator->() const {
        return function_;
    }

    void set(Function *function) {
        function_ = function;
    }

private:
    Function *function_;
};

} // namespace mini_llvm::mir
