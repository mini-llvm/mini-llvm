// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FunctionTransform : public ModuleTransform {
public:
    virtual bool runOnFunction(Function &F) = 0;
    bool runOnModule(Module &M) override;
};

} // namespace mini_llvm::ir
