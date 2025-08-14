// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/mir/Pass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT ModuleTransform : public Pass {
public:
    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::mir
