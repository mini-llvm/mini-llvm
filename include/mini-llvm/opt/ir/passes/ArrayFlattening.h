#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"

namespace mini_llvm::ir {

class ArrayFlattening final : public FunctionTransform {
public:
    explicit ArrayFlattening(int pointerSize)
        : pointerSize_(pointerSize) {}

    bool runOnFunction(Function &F) override;

    const char *name() const override {
        return "ArrayFlattening";
    }

private:
    int pointerSize_;
};

} // namespace mini_llvm::ir
