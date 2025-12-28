// SPDX-License-Identifier: MIT

#pragma once

#include <memory>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/opt/mir/FunctionAnalysis.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT BranchPredictionAnalysis final : public FunctionAnalysis {
public:
    BranchPredictionAnalysis();
    ~BranchPredictionAnalysis() override;
    void runOnFunction(const Function &F) override;
    bool predict(const BasicBlock &B, const BasicBlock &succ) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm::mir
