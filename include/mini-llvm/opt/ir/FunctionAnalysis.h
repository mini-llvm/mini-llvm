#pragma once

#include "mini-llvm/ir/Function.h"

namespace mini_llvm::ir {

class FunctionAnalysis {
public:
    virtual ~FunctionAnalysis() = default;
    virtual void runOnFunction(const Function &F) = 0;
};

} // namespace mini_llvm::ir
