// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/RedundantLoadElimination.h"

#include <cassert>
#include <memory>
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
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/AliasAnalysis.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

bool RedundantLoadElimination::runOnFunction(Function &F) {
    AliasAnalysis aa;
    aa.runOnFunction(F);

    bool changed = false;

    for (const BasicBlock &B : F) {
        std::unordered_set<const Store *> oldStores;
        std::unordered_set<const Load *> oldLoads;

        for (auto i = B.begin(); i != B.end();) {
            const Instruction &I = *i++;
            if (auto *store = dynamic_cast<const Store *>(&I)) {
                const Value *ptr = &*store->ptr();
                int size = store->value()->type()->size(pointerSize_);
                for (auto j = oldStores.begin(); j != oldStores.end();) {
                    const Store *oldStore = *j;
                    const Value *oldPtr = &*oldStore->ptr();
                    int oldSize = oldStore->value()->type()->size(pointerSize_);
                    if (aa.alias(*ptr, size, *oldPtr, oldSize) != AliasResult::kNoAlias) {
                        j = oldStores.erase(j);
                    } else {
                        ++j;
                    }
                }
                for (auto j = oldLoads.begin(); j != oldLoads.end();) {
                    const Load *oldLoad = *j;
                    const Value *oldPtr = &*oldLoad->ptr();
                    int oldSize = oldLoad->type()->size(pointerSize_);
                    if (aa.alias(*ptr, size, *oldPtr, oldSize) != AliasResult::kNoAlias) {
                        j = oldLoads.erase(j);
                    } else {
                        ++j;
                    }
                }
                oldStores.insert(store);
                continue;
            }
            if (auto *load = dynamic_cast<const Load *>(&I)) {
                const Value *ptr = &*load->ptr();
                std::unique_ptr<Type> type = load->type();
                int size = type->size(pointerSize_);
                Value *value = nullptr;
                for (const Store *oldStore : oldStores) {
                    const Value *oldPtr = &*oldStore->ptr();
                    std::unique_ptr<Type> oldType = oldStore->value()->type();
                    int oldSize = oldType->size(pointerSize_);
                    if (aa.alias(*ptr, size, *oldPtr, oldSize) == AliasResult::kMustAlias && *type == *oldType) {
                        value = &*oldStore->value();
                        break;
                    }
                }
                if (value) {
                    replaceAllUsesWith(*load, share(*value));
                    removeFromParent(*load);
                    changed = true;
                    continue;
                }
                for (const Load *oldLoad : oldLoads) {
                    const Value *oldPtr = &*oldLoad->ptr();
                    std::unique_ptr<Type> oldType = oldLoad->type();
                    int oldSize = oldType->size(pointerSize_);
                    if (aa.alias(*ptr, size, *oldPtr, oldSize) == AliasResult::kMustAlias && *type == *oldType) {
                        value = const_cast<Load *>(oldLoad);
                        break;
                    }
                }
                if (value) {
                    replaceAllUsesWith(*load, share(*value));
                    removeFromParent(*load);
                    changed = true;
                    continue;
                }
                oldLoads.insert(load);
                continue;
            }
            if (auto *call = dynamic_cast<const Call *>(&I)) {
                const Function &callee = *call->callee();
                if (callee.attr<ReadNone>() || callee.attr<ReadOnly>() || callee.attr<InaccessibleMemOnly>()) {
                    continue;
                }
                if (callee.attr<ArgMemOnly>() || callee.attr<InaccessibleMemOrArgMemOnly>()) {
                    for (auto j = oldStores.begin(); j != oldStores.end();) {
                        const Store *oldStore = *j;
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
                            j = oldStores.erase(j);
                        } else {
                            ++j;
                        }
                    }
                    for (auto j = oldLoads.begin(); j != oldLoads.end();) {
                        const Load *oldLoad = *j;
                        const Value *oldPtr = &*oldLoad->ptr();
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
                            j = oldLoads.erase(j);
                        } else {
                            ++j;
                        }
                    }
                    continue;
                }
                oldStores.clear();
                oldLoads.clear();
                continue;
            }
            if (dynamic_cast<const IndirectCall *>(&I)) {
                oldStores.clear();
                oldLoads.clear();
                continue;
            }
        }
    }

    assert(F.isWellFormed());
    return changed;
}
