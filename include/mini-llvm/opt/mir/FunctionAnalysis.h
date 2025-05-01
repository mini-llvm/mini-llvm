#pragma once

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/opt/mir/Pass.h"

namespace mini_llvm::mir {

class FunctionAnalysis : public Pass {
public:
    virtual void runOnFunction(const Function &F) = 0;
};

} // namespace mini_llvm::mir
