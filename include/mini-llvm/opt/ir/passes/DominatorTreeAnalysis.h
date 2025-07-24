#pragma once

#include <memory>
#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/opt/ir/FunctionAnalysis.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

struct DTNode {
    const BasicBlock *block;
    const DTNode *parent;
    std::vector<const DTNode *> children;
};

class MINI_LLVM_EXPORT DominatorTreeAnalysis final : public FunctionAnalysis {
public:
    DominatorTreeAnalysis();
    ~DominatorTreeAnalysis() override;
    void runOnFunction(const Function &F) override;
    const DTNode *node(const BasicBlock &v) const;
    bool dominates(const BasicBlock &u, const BasicBlock &v) const;
    bool dominates(const Instruction &u, const Instruction &v) const;

    const char *name() const override {
        return "DominatorTreeAnalysis";
    }

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm::ir
