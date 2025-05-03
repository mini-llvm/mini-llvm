#include "mini-llvm/opt/ir/passes/DeadStoreElimination.h"

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
    bool changed = false;

    for (const BasicBlock &B : F) {
        HashMap<const Value *, const Store *> lastStore;

        for (auto i = B.begin(); i != B.end();) {
            const Instruction &I = *i++;
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                if (auto j = lastStore.find(&*store->ptr()); j != lastStore.end()) {
                    removeFromParent(*j->second);
                    changed = true;
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

    return changed;
}
