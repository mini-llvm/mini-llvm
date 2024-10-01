#include "mini-llvm/opt/mir/FunctionTransform.h"

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Module.h"

using namespace mini_llvm::mir;

bool FunctionTransform::runOnModule(Module &M) {
    bool changed = false;
    for (Function &F : M.functions) {
        if (!F.empty()) {
            changed |= runOnFunction(F);
        }
    }
    return changed;
}
