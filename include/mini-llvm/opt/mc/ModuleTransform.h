// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT ModuleTransform {
public:
    virtual ~ModuleTransform() = default;

    ModuleTransform() = default;

    ModuleTransform(const ModuleTransform &) = delete;
    ModuleTransform &operator=(const ModuleTransform &) = delete;

    ModuleTransform(ModuleTransform &&) = delete;
    ModuleTransform &operator=(ModuleTransform &&) = delete;

    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::mc
