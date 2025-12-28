// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/BasicBlockMerging.h"

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction/Br.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

bool canMergeWithSuccessor(const BasicBlock &B, const HashMap<const BasicBlock *, int> &numPredecessors) {
    if (B.empty()) {
        return false;
    }
    if (!dynamic_cast<const Br *>(&B.back())) {
        return false;
    }
    const BasicBlock &succ = *static_cast<const Br *>(&B.back())->dest();
    if (numPredecessors[&succ] > 1) {
        return false;
    }
    return true;
}

} // namespace

bool BasicBlockMerging::runOnFunction(Function &F) {
    bool changed = false;

    HashMap<const BasicBlock *, int> numPredecessors;
    for (const BasicBlock &B : F) {
        numPredecessors.put(&B, 0);
    }
    for (const BasicBlock &B : F) {
        for (const BasicBlock *succ : successors(B)) {
            ++numPredecessors[succ];
        }
    }

    for (BasicBlock &B : F) {
        while (canMergeWithSuccessor(B, numPredecessors)) {
            BasicBlock &succ = *static_cast<const Br *>(&B.back())->dest();
            B.removeLast();
            while (!succ.empty()) {
                B.append(succ.removeFirst());
            }
            changed = true;
        }
    }

    for (Function::const_iterator i = F.begin(); i != F.end();) {
        if (i->empty()) {
            F.remove(i++);
        } else {
            ++i;
        }
    }

    return changed;
}
