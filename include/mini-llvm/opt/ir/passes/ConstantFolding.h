// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT ConstantFolding final : public FunctionTransform {
public:
    bool runOnFunction(Function &F) override;
};

} // namespace mini_llvm::ir
