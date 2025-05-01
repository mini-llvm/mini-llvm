#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/Pass.h"

namespace mini_llvm::ir {

class ModuleTransform : public Pass {
public:
    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::ir
