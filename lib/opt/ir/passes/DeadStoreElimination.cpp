#include "mini-llvm/opt/ir/passes/DeadStoreElimination.h"

#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

bool DeadStoreElimination::runOnFunction(Function &F) {
    std::vector<const Instruction *> remove;

    for (const BasicBlock &B : F) {
        HashMap<const Value *, const Store *> lastStore;

        for (const Instruction &I : B) {
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                if (auto i = lastStore.find(&*store->ptr()); i != lastStore.end()) {
                    remove.push_back(i->second);
                }
                lastStore(&*store->ptr()) = &*store;
                continue;
            }
            if (auto *load = dynamic_cast<const Load *>(&I)) {
                lastStore.erase(&*load->ptr());
            }
            if (dynamic_cast<const Call *>(&I)) {
                lastStore.clear();
                continue;
            }
        }
    }

    bool changed = false;

    for (const Instruction *I : remove) {
        removeFromParent(*I);
        changed = true;
    }

    return changed;
}
