#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"

namespace mini_llvm::ir {

class RedundantLoadElimination : public FunctionTransform {
public:
    explicit RedundantLoadElimination(int pointerSize)
        : pointerSize_(pointerSize) {}

    bool runOnFunction(Function &F) override;

    const char *name() const override {
        return "RedundantLoadElimination";
    }

private:
    int pointerSize_;
};

} // namespace mini_llvm::ir
