// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT RedundantLoadElimination : public FunctionTransform {
public:
    explicit RedundantLoadElimination(int pointerSize)
        : pointerSize_(pointerSize) {}

    bool runOnFunction(Function &F) override;

private:
    int pointerSize_;
};

} // namespace mini_llvm::ir
