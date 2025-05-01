#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"

namespace mini_llvm::ir {

class DeadCodeElimination final : public FunctionTransform {
public:
    bool runOnFunction(Function &F) override;

    const char *name() const override {
        return "DeadCodeElimination";
    }
};

} // namespace mini_llvm::ir
