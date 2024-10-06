#pragma once

#include "mini-llvm/mir/Function.h"

namespace mini_llvm::mir {

class FunctionAnalysis {
public:
    virtual ~FunctionAnalysis() = default;
    virtual void runOnFunction(const Function &F) = 0;
};

} // namespace mini_llvm::mir
