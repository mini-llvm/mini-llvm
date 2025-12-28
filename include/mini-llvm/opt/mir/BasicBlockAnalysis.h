// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT BasicBlockAnalysis {
public:
    virtual ~BasicBlockAnalysis() = default;

    BasicBlockAnalysis() = default;

    BasicBlockAnalysis(const BasicBlockAnalysis &) = delete;
    BasicBlockAnalysis &operator=(const BasicBlockAnalysis &) = delete;

    BasicBlockAnalysis(BasicBlockAnalysis &&) = delete;
    BasicBlockAnalysis &operator=(BasicBlockAnalysis &&) = delete;

    virtual void runOnBasicBlock(const BasicBlock &B) = 0;
};

} // namespace mini_llvm::mir
