// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/Pass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FunctionAnalysis : public Pass {
public:
    virtual void runOnFunction(const Function &F) = 0;
};

} // namespace mini_llvm::ir
