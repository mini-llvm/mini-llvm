// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/DeadCodeElimination.h"

#include <unordered_set>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/opt/mir/passes/LiveVariableAnalysis.h"

using namespace mini_llvm::mir;

namespace {

bool isCritical(const Instruction &I, const std::unordered_set<Register *> &liveOut) {
    if (I.hasSideEffects()) {
        return true;
    }
    for (Register *reg : def(I)) {
        if (liveOut.contains(reg)) {
            return true;
        }
    }
    return false;
}

} // namespace

bool DeadCodeElimination::runOnFunction(Function &F) {
    bool changed = false;

    bool changed2;
    do {
        changed2 = false;

        LiveVariableAnalysis liveVars;
        liveVars.runOnFunction(F);

        for (BasicBlock &B : F) {
            for (BasicBlock::const_iterator i = B.begin(); i != B.end();) {
                if (!isCritical(*i, liveVars.liveOut(*i))) {
                    B.remove(i++);
                    changed2 = true;
                    continue;
                }
                ++i;
            }
        }

        changed |= changed2;
    } while (changed2);

    return changed;
}
