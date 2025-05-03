#include "mini-llvm/opt/ir/passes/RedundantLoadElimination.h"

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

bool RedundantLoadElimination::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        HashMap<Value *, Value *> lastValue;
        HashMap<Value *, Load *> lastLoad;

        for (auto i = B.begin(); i != B.end();) {
            Instruction &I = *i++;
            if (auto *store = dynamic_cast<Store *>(&I)) {
                lastValue(&*store->ptr()) = &*store->value();
                continue;
            }
            if (dynamic_cast<Call *>(&I)) {
                lastValue.clear();
                lastLoad.clear();
                continue;
            }
            if (auto *load = dynamic_cast<Load *>(&I)) {
                if (auto j = lastValue.find(&*load->ptr()); j != lastValue.end()) {
                    replaceAllUsesWith(*load, share(*j->second));
                    removeFromParent(*load);
                    changed = true;
                    continue;
                }
                if (auto j = lastLoad.find(&*load->ptr()); j != lastLoad.end()) {
                    replaceAllUsesWith(*load, share(*j->second));
                    removeFromParent(*load);
                    changed = true;
                    continue;
                }
                lastLoad(&*load->ptr()) = &*load;
                continue;
            }
        }
    }

    return changed;
}
