// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/mir/ModuleTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT FunctionTransform : public ModuleTransform {
public:
    virtual bool runOnFunction(Function &F) = 0;
    bool runOnModule(Module &M) override;
};

} // namespace mini_llvm::mir
