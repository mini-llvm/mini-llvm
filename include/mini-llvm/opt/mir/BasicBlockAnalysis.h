#pragma once

#include "mini-llvm/mir/BasicBlock.h"

namespace mini_llvm::mir {

class BasicBlockAnalysis {
public:
    virtual ~BasicBlockAnalysis() = default;
    virtual void runOnBasicBlock(const BasicBlock &B) = 0;
};

} // namespace mini_llvm::mir
