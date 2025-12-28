// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/BasicBlockTransform.h"

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"

using namespace mini_llvm::mir;

bool BasicBlockTransform::runOnFunction(Function &F) {
    bool changed = false;
    for (BasicBlock &B : F) {
        changed |= runOnBasicBlock(B);
    }
    return changed;
}
