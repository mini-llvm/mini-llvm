#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"

namespace mini_llvm::ir {

class FunctionTransform : public ModuleTransform {
public:
    virtual bool runOnFunction(Function &F) = 0;
    bool runOnModule(Module &M) override;
};

} // namespace mini_llvm::ir
