// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT FunctionAnalysis {
public:
    virtual ~FunctionAnalysis() = default;

    FunctionAnalysis() = default;

    FunctionAnalysis(const FunctionAnalysis &) = delete;
    FunctionAnalysis &operator=(const FunctionAnalysis &) = delete;

    FunctionAnalysis(FunctionAnalysis &&) = delete;
    FunctionAnalysis &operator=(FunctionAnalysis &&) = delete;

    virtual void runOnFunction(const Function &F) = 0;
};

} // namespace mini_llvm::mir
