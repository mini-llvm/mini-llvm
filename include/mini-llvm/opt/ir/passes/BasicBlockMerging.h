#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"

namespace mini_llvm::ir {

class BasicBlockMerging final : public FunctionTransform {
public:
    bool runOnFunction(Function &F) override;
};

} // namespace mini_llvm::ir
