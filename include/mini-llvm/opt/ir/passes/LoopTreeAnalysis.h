// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <unordered_set>
#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionAnalysis.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

struct Loop {
    std::unordered_set<const BasicBlock *> blocks;
    const BasicBlock *header;
};

struct LTNode {
    const Loop *loop;
    const LTNode *parent;
    std::vector<const LTNode *> children;
};

class MINI_LLVM_EXPORT LoopTreeAnalysis final : public FunctionAnalysis {
public:
    LoopTreeAnalysis();
    ~LoopTreeAnalysis() override;
    void runOnFunction(const Function &F) override;
    const std::vector<Loop> &loops() const;
    const LTNode *node(const Loop &loop) const;
    const LTNode *rootNode() const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm::ir
