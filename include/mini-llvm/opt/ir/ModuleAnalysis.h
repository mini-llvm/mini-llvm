#pragma once

#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

class ModuleAnalysis {
public:
    virtual ~ModuleAnalysis() = default;
    virtual void runOnModule(const Module &M) = 0;
};

} // namespace mini_llvm::ir
