// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/DeadStoreElimination.h"

#include <cassert>
#include <unordered_set>

#include "mini-llvm/ir/Attribute/ArgMemOnly.h"
#include "mini-llvm/ir/Attribute/InaccessibleMemOnly.h"
#include "mini-llvm/ir/Attribute/InaccessibleMemOrArgMemOnly.h"
#include "mini-llvm/ir/Attribute/ReadNone.h"
#include "mini-llvm/ir/Attribute/ReadOnly.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/IndirectCall.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/AliasAnalysis.h"

using namespace mini_llvm::ir;

bool DeadStoreElimination::runOnFunction(Function &F) {
    AliasAnalysis aa;
    aa.runOnFunction(F);

    bool changed = false;

    for (const BasicBlock &B : F) {
        std::unordered_set<const Store *> oldStores;

        for (const Instruction &I : B) {
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                const Value *ptr = &*store->ptr();
                int size = store->value()->type()->size(pointerSize_);
                for (auto i = oldStores.begin(); i != oldStores.end();) {
                    const Store *oldStore = *i;
                    const Value *oldPtr = &*oldStore->ptr();
                    int oldSize = oldStore->value()->type()->size(pointerSize_);
                    AliasResult result = aa.alias(*ptr, size, *oldPtr, oldSize);
                    if (result != AliasResult::kNoAlias) {
                        i = oldStores.erase(i);
                    } else {
                        ++i;
                    }
                    if (result == AliasResult::kMustAlias) {
                        removeFromParent(*oldStore);
                        changed = true;
                    }
                }
                oldStores.insert(store);
                continue;
            }
            if (auto *load = dynamic_cast<const Load *>(&I)) {
                const Value *ptr = &*load->ptr();
                int size = load->type()->size(pointerSize_);
                for (auto i = oldStores.begin(); i != oldStores.end();) {
                    const Store *oldStore = *i;
                    const Value *oldPtr = &*oldStore->ptr();
                    int oldSize = oldStore->value()->type()->size(pointerSize_);
                    AliasResult result = aa.alias(*ptr, size, *oldPtr, oldSize);
                    if (result != AliasResult::kNoAlias) {
                        i = oldStores.erase(i);
                    } else {
                        ++i;
                    }
                }
                continue;
            }
            if (auto *call = dynamic_cast<const Call *>(&I)) {
                const Function &callee = *call->callee();
                if (callee.attr<ReadNone>() || callee.attr<ReadOnly>() || callee.attr<InaccessibleMemOnly>()) {
                    continue;
                }
                if (callee.attr<ArgMemOnly>() || callee.attr<InaccessibleMemOrArgMemOnly>()) {
                    for (auto i = oldStores.begin(); i != oldStores.end();) {
                        const Store *oldStore = *i;
                        const Value *oldPtr = &*oldStore->ptr();
                        bool alias = false;
                        for (const Use<Value> &arg : args(*call)) {
                            if (*arg->type() == Ptr()) {
                                AliasResult result = aa.alias(*arg, *oldPtr);
                                if (result != AliasResult::kNoAlias) {
                                    alias = true;
                                    break;
                                }
                            }
                        }
                        if (alias) {
                            i = oldStores.erase(i);
                        } else {
                            ++i;
                        }
                    }
                    continue;
                }
                oldStores.clear();
                continue;
            }
            if (dynamic_cast<const IndirectCall *>(&I)) {
                oldStores.clear();
                continue;
            }
        }
    }

    assert(F.isWellFormed());
    return changed;
}
