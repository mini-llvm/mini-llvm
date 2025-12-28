// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/UnreachableBlockElimination.h"

#include <queue>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"

using namespace mini_llvm::mir;

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

    for (Function::const_iterator i = F.begin(); i != F.end();) {
        if (!visited.contains(&*i)) {
            F.remove(i++);
            changed = true;
            continue;
        }
        ++i;
    }

    return changed;
}
