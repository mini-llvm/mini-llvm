#pragma once

#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/mir/Pass.h"

namespace mini_llvm::mir {

class ModuleTransform : Pass {
public:
    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::mir
