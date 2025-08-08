// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/mir/Pass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT ModuleAnalysis : public Pass {
public:
    virtual void runOnModule(const Module &M) = 0;
};

} // namespace mini_llvm::mir
