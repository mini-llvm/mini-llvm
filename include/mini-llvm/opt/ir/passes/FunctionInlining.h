// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FunctionInlining final : public FunctionTransform {
public:
    bool runOnFunction(Function &F) override;

    const char *name() const override {
        return "FunctionInlining";
    }
};

} // namespace mini_llvm::ir
