#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT GlobalDeadCodeElimination final : public ModuleTransform {
public:
    bool runOnModule(Module &M) override;

    const char *name() const override {
        return "GlobalDeadCodeElimination";
    }
};

} // namespace mini_llvm::ir
