#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"

namespace mini_llvm::ir {

class AttributeDeduction final : public ModuleTransform {
public:
    bool runOnModule(Module &M) override;
};

} // namespace mini_llvm::ir
