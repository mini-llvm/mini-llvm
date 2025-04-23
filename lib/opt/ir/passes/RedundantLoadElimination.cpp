#include "mini-llvm/opt/ir/passes/RedundantLoadElimination.h"

#include <vector>

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

    std::vector<Instruction *> remove;

    for (BasicBlock &B : F) {
        HashMap<Value *, Value *> lastValue;
        HashMap<Value *, Load *> lastLoad;

        for (Instruction &I : B) {
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
                if (auto i = lastValue.find(&*load->ptr()); i != lastValue.end()) {
                    changed |= replaceAllUsesWith(*load, share(*i->second));
                    remove.push_back(load);
                    continue;
                }
                if (auto i = lastLoad.find(&*load->ptr()); i != lastLoad.end()) {
                    changed |= replaceAllUsesWith(*load, share(*i->second));
                    remove.push_back(load);
                    continue;
                }
                lastLoad(&*load->ptr()) = &*load;
                continue;
            }
        }
    }

    for (Instruction *I : remove) {
        removeFromParent(*I);
        changed = true;
    }

    return changed;
}
