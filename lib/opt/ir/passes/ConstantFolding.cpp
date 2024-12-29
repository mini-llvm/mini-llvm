#include "mini-llvm/opt/ir/passes/ConstantFolding.h"

#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

using namespace mini_llvm::ir;

namespace {

void dfs(const DominatorTreeNode *node, bool &changed) {
    std::vector<const Instruction *> remove;

    for (const Instruction &I : *node->block) {
        if (I.isFoldable()) {
            changed |= replaceAllUsesWith(I, I.fold());
            remove.push_back(&I);
        }
    }

    for (const Instruction *I : remove) {
        removeFromParent(*I);
        changed = true;
    }

    for (const DominatorTreeNode *child : node->children) {
        dfs(child, changed);
    }
}

} // namespace

bool ConstantFolding::runOnFunction(Function &F) {
    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    bool changed = false;

    dfs(domTree.node(F.entry()), changed);

    return changed;
}
