// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/Pass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT ModuleAnalysis : public Pass {
public:
    virtual void runOnModule(const Module &M) = 0;
};

} // namespace mini_llvm::ir
