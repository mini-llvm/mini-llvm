#pragma once

#include <memory>
#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionAnalysis.h"

namespace mini_llvm::ir {

struct Loop {
    std::unordered_set<const BasicBlock *> blocks;
    const BasicBlock *header;
};

struct LoopTreeNode {
    const Loop *loop;
    const LoopTreeNode *parent;
    std::vector<const LoopTreeNode *> children;
};

class LoopTreeAnalysis final : public FunctionAnalysis {
public:
    LoopTreeAnalysis();
    ~LoopTreeAnalysis() override;
    void runOnFunction(const Function &F) override;
    const std::vector<Loop> &loops() const;
    const LoopTreeNode *node(const Loop &loop) const;
    const LoopTreeNode *rootNode() const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm::ir
