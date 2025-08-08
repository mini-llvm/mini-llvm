// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/FunctionTransform.h"

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Module.h"

using namespace mini_llvm::mir;

bool FunctionTransform::runOnModule(Module &M) {
    bool changed = false;
    for (Function &F : functions(M)) {
        if (!F.isDeclaration()) {
            changed |= runOnFunction(F);
        }
    }
    return changed;
}
