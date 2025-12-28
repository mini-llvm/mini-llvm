// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/opt/mc/ModuleTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT GlobalValueTransform : public ModuleTransform {
public:
    virtual bool runOnGlobalValue(GlobalValue &G) = 0;
    bool runOnModule(Module &M) override;
};

} // namespace mini_llvm::mc
