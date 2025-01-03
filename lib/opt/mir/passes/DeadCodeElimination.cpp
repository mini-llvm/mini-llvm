#include "mini-llvm/opt/mir/passes/DeadCodeElimination.h"

#include <unordered_set>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/opt/mir/passes/LiveVariableAnalysis.h"

using namespace mini_llvm::mir;

namespace {

bool canRemove(const Instruction &I, const std::unordered_set<Register *> &liveOut) {
    if (I.hasSideEffects()) {
        return false;
    }
    for (Register *reg : def(I)) {
        if (liveOut.contains(reg)) {
            return false;
        }
    }
    return true;
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
            std::vector<BasicBlock::const_iterator> remove;

            for (BasicBlock::const_iterator i = B.begin(), e = B.end(); i != e; ++i) {
                if (canRemove(*i, liveVars.liveOut(*i))) {
                    remove.push_back(i);
                }
            }

            for (BasicBlock::const_iterator i : remove) {
                B.remove(i);
                changed2 = true;
            }
        }

        changed |= changed2;
    } while (changed2);

    return changed;
}
