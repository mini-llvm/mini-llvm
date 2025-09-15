// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT ModuleAnalysis {
public:
    virtual ~ModuleAnalysis() = default;

    ModuleAnalysis() = default;

    ModuleAnalysis(const ModuleAnalysis &) = delete;
    ModuleAnalysis &operator=(const ModuleAnalysis &) = delete;

    ModuleAnalysis(ModuleAnalysis &&) = delete;
    ModuleAnalysis &operator=(ModuleAnalysis &&) = delete;

    virtual void runOnModule(const Module &M) = 0;
};

} // namespace mini_llvm::ir
