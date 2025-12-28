// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FunctionInlining final : public FunctionTransform {
public:
    FunctionInlining(size_t calleeThreshold, size_t callerThreshold)
        : calleeThreshold_(calleeThreshold), callerThreshold_(callerThreshold) {}

    bool runOnFunction(Function &F) override;

private:
    size_t calleeThreshold_;
    size_t callerThreshold_;
};

} // namespace mini_llvm::ir
