// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/opt/mir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT DeadCodeElimination final : public FunctionTransform {
public:
    bool runOnFunction(Function &F) override;

    const char *name() const override {
        return "DeadCodeElimination";
    }
};

} // namespace mini_llvm::mir
