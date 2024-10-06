#include "mini-llvm/opt/mir/passes/DeadCodeElimination.h"

#include <algorithm>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/opt/mir/passes/LiveVariableAnalysis.h"

using namespace mini_llvm::mir;

bool DeadCodeElimination::runOnFunction(Function &F) {
    bool changed = false;

    bool changed2;
    do {
        changed2 = false;

        LiveVariableAnalysis liveVars;
        liveVars.runOnFunction(F);

        for (BasicBlock &B : F) {
            std::vector<BasicBlock::const_iterator> remove;

            for (BasicBlock::const_iterator i = B.begin(); i != B.end(); ++i) {
                if (!i->hasSideEffects()
                        && std::ranges::none_of(def(*i), [&](Register *reg) { return liveVars.liveOut(*i).contains(reg); })) {
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
