// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"

#include <cassert>
#include <queue>
#include <unordered_set>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Phi.h"

using namespace mini_llvm::ir;

bool UnreachableBlockElimination::runOnFunction(Function &F) {
    std::unordered_set<const BasicBlock *> visited;
    std::queue<const BasicBlock *> Q;
    visited.insert(&F.entry());
    Q.push(&F.entry());
    while (!Q.empty()) {
        const BasicBlock *u = Q.front();
        Q.pop();
        for (const BasicBlock *v : successors(*u)) {
            if (visited.insert(v).second) {
                Q.push(v);
            }
        }
    }

    bool changed = false;

    for (BasicBlock &B : F) {
        for (Instruction &I : B) {
            if (auto *phi = dynamic_cast<Phi *>(&I)) {
                for (auto i = phi->incoming_begin(); i != phi->incoming_end();) {
                    if (!visited.contains(&*i->block)) {
                        phi->removeIncoming(i++);
                    } else {
                        ++i;
                    }
                }
            }
        }
    }

    for (auto i = F.begin(); i != F.end();) {
        const BasicBlock &B = *i++;
        if (!visited.contains(&B)) {
            removeFromParent(B);
            changed = true;
        }
    }

    assert(F.isWellFormed());
    return changed;
}
