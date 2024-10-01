#pragma once

#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

class ModuleTransform {
public:
    virtual ~ModuleTransform() = default;
    virtual bool runOnModule(Module &M) = 0;
};

} // namespace mini_llvm::ir
