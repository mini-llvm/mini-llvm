#pragma once

#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/mir/Pass.h"

namespace mini_llvm::mir {

class ModuleAnalysis : public Pass {
public:
    virtual void runOnModule(const Module &M) = 0;
};

} // namespace mini_llvm::mir
