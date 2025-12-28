// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT RISCVPassManager {
public:
    void runBeforeRegisterAllocation(Module &M) const;
    void runAfterRegisterAllocation(Module &M) const;
};

} // namespace mini_llvm::mir
