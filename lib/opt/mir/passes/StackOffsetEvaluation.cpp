// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/StackOffsetEvaluation.h"

#include <memory>

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/IntegerImmediate.h"
#include "mini-llvm/mir/StackOffsetImmediate.h"

using namespace mini_llvm::mir;

bool StackOffsetEvaluation::runOnFunction(Function &F) {
    if (F.stackFrame().empty()) {
        return false;
    }

    for (BasicBlock &B : F) {
        for (Instruction &I : B) {
            for (ImmediateOperand *op : I.immOps()) {
                if (auto *imm = dynamic_cast<const StackOffsetImmediate *>(&**op)) {
                    op->set(std::make_unique<IntegerImmediate>(imm->value()));
                }
            }
        }
    }

    F.stackFrame().clear();

    return true;
}
