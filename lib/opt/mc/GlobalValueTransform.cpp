// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mc/GlobalValueTransform.h"

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/mc/Module.h"

using namespace mini_llvm::mc;

bool GlobalValueTransform::runOnModule(Module &M) {
    bool changed = false;
    for (GlobalValue &G : M) {
        changed |= runOnGlobalValue(G);
    }
    return changed;
}
