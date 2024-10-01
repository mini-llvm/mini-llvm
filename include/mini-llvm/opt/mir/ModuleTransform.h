#pragma once

#include "mini-llvm/mir/Module.h"

namespace mini_llvm::mir {

class ModuleTransform {
public:
    virtual ~ModuleTransform() = default;
    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::mir
