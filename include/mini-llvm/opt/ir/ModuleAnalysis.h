#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/Pass.h"

namespace mini_llvm::ir {

class ModuleAnalysis : public Pass {
public:
    virtual void runOnModule(const Module &M) = 0;
};

} // namespace mini_llvm::ir
