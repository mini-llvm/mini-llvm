#pragma once

#include "mini-llvm/mir/Module.h"

namespace mini_llvm::mir {

class RISCVPassManager {
public:
    void runBeforeRegisterAllocation(Module &M) const;
    void runAfterRegisterAllocation(Module &M) const;
};

} // namespace mini_llvm::mir
