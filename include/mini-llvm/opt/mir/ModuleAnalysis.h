#pragma once

#include "mini-llvm/mir/Module.h"

namespace mini_llvm::mir {

class ModuleAnalysis {
public:
    virtual ~ModuleAnalysis() = default;
    virtual void runOnModule(const Module &M) = 0;
};

} // namespace mini_llvm::mir
