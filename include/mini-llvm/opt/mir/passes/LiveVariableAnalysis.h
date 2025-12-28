// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/opt/mir/FunctionAnalysis.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT LiveVariableAnalysis final : public FunctionAnalysis {
public:
    LiveVariableAnalysis();
    ~LiveVariableAnalysis() override;
    void runOnFunction(const Function &F) override;
    std::unordered_set<Register *> liveIn(const BasicBlock &B) const;
    std::unordered_set<Register *> liveOut(const BasicBlock &B) const;
    std::unordered_set<Register *> liveIn(const Instruction &I) const;
    std::unordered_set<Register *> liveOut(const Instruction &I) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm::mir
