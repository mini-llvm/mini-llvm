#pragma once

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/opt/mc/Pass.h"

namespace mini_llvm::mc {

class ModuleTransform : public Pass {
public:
    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::mc
