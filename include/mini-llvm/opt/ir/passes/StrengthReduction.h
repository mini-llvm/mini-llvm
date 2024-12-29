#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"

namespace mini_llvm::ir {

class StrengthReduction final : public FunctionTransform {
public:
    explicit StrengthReduction(size_t mulThreshold) : mulThreshold_(mulThreshold) {}

    bool runOnFunction(Function &F) override;

private:
    size_t mulThreshold_;
};

} // namespace mini_llvm::ir
