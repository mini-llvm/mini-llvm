// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/opt/mir/Pass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT BasicBlockAnalysis : public Pass {
public:
    virtual void runOnBasicBlock(const BasicBlock &B) = 0;
};

} // namespace mini_llvm::mir
