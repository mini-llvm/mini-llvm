#pragma once

#include <memory>
#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/opt/ir/FunctionAnalysis.h"

namespace mini_llvm::ir {

struct DominatorTreeNode {
    const BasicBlock *block;
    const DominatorTreeNode *parent;
    std::vector<const DominatorTreeNode *> children;
};

class DominatorTreeAnalysis final : public FunctionAnalysis {
public:
    DominatorTreeAnalysis();
    ~DominatorTreeAnalysis() override;
    void runOnFunction(const Function &F) override;
    const DominatorTreeNode *node(const BasicBlock &v) const;
    bool dominates(const BasicBlock &u, const BasicBlock &v) const;
    bool dominates(const Instruction &u, const Instruction &v) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm::ir
