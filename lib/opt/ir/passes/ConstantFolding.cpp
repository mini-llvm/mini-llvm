// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/ConstantFolding.h"

#include <cassert>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

using namespace mini_llvm::ir;

namespace {

void dfs(const DTNode *node, bool &changed) {
    for (auto i = node->block->begin(); i != node->block->end();) {
        const Instruction &I = *i++;
        if (I.isFoldable()) {
            replaceAllUsesWith(I, I.fold());
            removeFromParent(I);
            changed = true;
        }
    }

    for (const DTNode *child : node->children) {
        dfs(child, changed);
    }
}

} // namespace

bool ConstantFolding::runOnFunction(Function &F) {
    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    bool changed = false;

    dfs(domTree.node(F.entry()), changed);

    assert(F.isWellFormed());
    return changed;
}
