#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/opt/mir/Pass.h"

namespace mini_llvm::mir {

class BasicBlockAnalysis : public Pass {
public:
    virtual void runOnBasicBlock(const BasicBlock &B) = 0;
};

} // namespace mini_llvm::mir
