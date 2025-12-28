// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/JumpThreading.h"

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Instruction/Br.h"
#include "mini-llvm/mir/Instruction/Terminator.h"

using namespace mini_llvm::mir;

namespace {

BasicBlock *follow(BasicBlock *B) {
    if (B->size() == 1) {
        if (auto *br = dynamic_cast<const Br *>(&B->back())) {
            if (&*br->dest() != B) {
                return &*br->dest();
            }
        }
    }
    return nullptr;
}

} // namespace

bool JumpThreading::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        for (BasicBlockOperand *op : static_cast<Terminator *>(&B.back())->blockOps()) {
            BasicBlock *dest = &**op;
            while (BasicBlock *newDest = follow(dest)) {
                dest = newDest;
            }
            if (dest != &**op) {
                op->set(dest);
                changed = true;
            }
        }
    }

    return changed;
}
