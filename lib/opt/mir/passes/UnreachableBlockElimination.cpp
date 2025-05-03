#include "mini-llvm/opt/mir/passes/UnreachableBlockElimination.h"

#include <queue>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"

using namespace mini_llvm::mir;

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

    for (Function::const_iterator i = F.begin(); i != F.end();) {
        if (!S.contains(&*i)) {
            F.remove(i++);
            changed = true;
            continue;
        }
        ++i;
    }

    return changed;
}
