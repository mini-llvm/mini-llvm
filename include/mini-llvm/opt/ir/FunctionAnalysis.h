#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/Pass.h"

namespace mini_llvm::ir {

class FunctionAnalysis : public Pass {
public:
    virtual void runOnFunction(const Function &F) = 0;
};

} // namespace mini_llvm::ir
