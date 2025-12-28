// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/opt/mir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT BasicBlockTransform : public FunctionTransform {
public:
    virtual bool runOnBasicBlock(BasicBlock &B) = 0;
    bool runOnFunction(Function &F) override;
};

} // namespace mini_llvm::mir
