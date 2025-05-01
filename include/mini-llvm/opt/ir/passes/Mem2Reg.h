#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"

namespace mini_llvm::ir {

class Mem2Reg final : public FunctionTransform {
public:
    bool runOnFunction(Function &F) override;

    const char *name() const override {
        return "Mem2Reg";
    }
};

} // namespace mini_llvm::ir
