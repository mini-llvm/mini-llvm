#include "mini-llvm/opt/ir/passes/PoisonFreeze.h"

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Use.h"

using namespace mini_llvm::ir;

bool PoisonFreeze::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        for (Instruction &I : B) {
            for (UseBase *op : I.operands()) {
                if (dynamic_cast<const PoisonValue *>(&**op)) {
                    op->set((*op)->type()->zeroValue());
                    changed = true;
                }
            }
        }
    }

    return changed;
}
