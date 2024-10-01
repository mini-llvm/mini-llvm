#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"

#include <queue>
#include <unordered_set>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Phi.h"

using namespace mini_llvm::ir;

bool UnreachableBlockElimination::runOnFunction(Function &F) {
    std::unordered_set<const BasicBlock *> S;
    std::queue<const BasicBlock *> Q;
    S.insert(&F.entry());
    Q.push(&F.entry());
    while (!Q.empty()) {
        const BasicBlock *u = Q.front();
        Q.pop();
        for (const BasicBlock *v : successors(*u)) {
            if (!S.contains(v)) {
                S.insert(v);
                Q.push(v);
            }
        }
    }

    bool changed = false;

    std::vector<const BasicBlock *> remove;

    for (const BasicBlock &B : F) {
        if (!S.contains(&B)) {
            remove.push_back(&B);
        }
    }

    for (BasicBlock &B : F) {
        for (Instruction &I : B) {
            if (auto *phi = dynamic_cast<Phi *>(&I)) {
                for (const BasicBlock *BB : remove) {
                    if (hasIncomingBlock(*phi, *BB)) {
                        removeIncomingBlock(*phi, *BB);
                        changed = true;
                    }
                }
            }
        }
    }

    for (const BasicBlock *B : remove) {
        removeFromParent(*B);
        changed = true;
    }

    return changed;
}
