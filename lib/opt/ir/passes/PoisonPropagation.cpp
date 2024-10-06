#include "mini-llvm/opt/ir/passes/PoisonPropagation.h"

#include <algorithm>
#include <memory>
#include <vector>

#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Operator.h"
#include "mini-llvm/ir/Instruction/Select.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

using namespace mini_llvm::ir;

namespace {

bool isPoison(const Instruction &I) {
    if (dynamic_cast<const Operator *>(&I) || dynamic_cast<const GetElementPtr *>(&I) || dynamic_cast<const Load *>(&I)) {
        return std::ranges::any_of(I.operands(), [](const UseBase *op) {
            return dynamic_cast<const PoisonValue *>(&**op);
        });
    }

    if (auto *select = dynamic_cast<const Select *>(&I)) {
        return dynamic_cast<const PoisonValue *>(&*select->cond());
    }

    return false;
}

void dfs(const DominatorTreeNode *node, bool &changed, std::vector<const Instruction *> &remove) {
    for (const Instruction &I : *node->block) {
        if (isPoison(I)) {
            changed |= replaceAllUsesWith(I, std::make_shared<PoisonValue>(I.type()));
            remove.push_back(&I);
        }
    }

    for (const DominatorTreeNode *child : node->children) {
        dfs(child, changed, remove);
    }
}

} // namespace

bool PoisonPropagation::runOnFunction(Function &F) {
    bool changed = false;

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    std::vector<const Instruction *> remove;

    dfs(domTree.node(F.entry()), changed, remove);

    for (const Instruction *I : remove) {
        removeFromParent(*I);
        changed = true;
    }

    return changed;
}
