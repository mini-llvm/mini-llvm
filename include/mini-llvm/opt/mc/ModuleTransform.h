// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/opt/mc/Pass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT ModuleTransform : public Pass {
public:
    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::mc
