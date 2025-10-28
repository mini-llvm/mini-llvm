// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/FunctionInlining.h"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Attribute/AlwaysInline.h"
#include "mini-llvm/ir/Attribute/NoInline.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

bool hasMoreThanNInstructions(const Function &F, size_t n) {
    size_t count = 0;
    for (const BasicBlock &B : F) {
        count += B.size();
        if (count > n) {
            return true;
        }
    }
    return false;
}

bool isRecursive(const Function &F) {
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (auto *call = dynamic_cast<const Call *>(&I)) {
                if (&*call->callee() == &F) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool shouldInline(const Call &call, size_t calleeThreshold, size_t callerThreshold) {
    if (call.callee()->isDeclaration()) return false;
    if (call.callee()->attr<NoInline>()) return false;
    if (call.callee()->attr<AlwaysInline>()) return true;
    if (isRecursive(*call.callee())) return false;
    if (hasMoreThanNInstructions(*call.callee(), calleeThreshold)) return false;
    if (hasMoreThanNInstructions(*call.parent()->parent(), callerThreshold)) return false;
    return true;
}

BasicBlock *splitBefore(BasicBlock::const_iterator i) {
    BasicBlock *B = i->parent();
    Function *F = B->parent();

    BasicBlock *B2 = &F->append();

    for (BasicBlock *succ : successors(*B)) {
        for (Instruction &I : *succ) {
            if (auto *phi = dynamic_cast<Phi *>(&I)) {
                if (auto j = phi->findIncoming(*B); j != phi->incoming_end()) {
                    std::shared_ptr<Value> value = share(*j->value);
                    phi->removeIncoming(j);
                    phi->addIncoming(weaken(*B2), std::move(value));
                }
            }
        }
    }

    for (auto j = i; j != B->end();) {
        std::shared_ptr<Instruction> I = share(*const_cast<Instruction *>(&*j++));
        removeFromParent(*I);
        B2->append(std::move(I));
    }

    return B2;
}

} // namespace

bool FunctionInlining::runOnFunction(Function &F) {
    bool changed = false;

    for (auto i = F.begin(); i != F.end(); ++i) {
        for (auto j = i->begin(), e = i->end(); j != e; ++j) {
            if (auto *call = dynamic_cast<const Call *>(&*j)) {
                const Function *callee = &*call->callee();
                if (shouldInline(*call, calleeThreshold_, callerThreshold_)) {
                    BasicBlock *B = splitBefore(std::next(j));

                    if (*callee->functionType()->returnType() != Void()) {
                        Phi &phi = B->prepend(std::make_shared<Phi>(callee->functionType()->returnType()));
                        replaceAllUsesWith(*call, weaken(phi));
                    }

                    HashMap<const Value *, Value *> valueMap;

                    for (auto [calleeArg, callArg] : std::views::zip(args(*callee), args(*call))) {
                        valueMap.put(&calleeArg, &*callArg);
                    }

                    for (const BasicBlock &callee_B : *callee) {
                        valueMap.put(&callee_B, &F.append());
                    }

                    BasicBlock *calleeEntry = static_cast<BasicBlock *>(valueMap[&callee->entry()]);
                    addToParent(*call, std::make_shared<Br>(weaken(*calleeEntry)));

                    std::vector<Instruction *> cloned;
                    std::vector<std::pair<BasicBlock *, Value *>> exits;

                    for (const BasicBlock &callee_B : *callee) {
                        for (const Instruction &callee_I : callee_B) {
                            BasicBlock *caller_B = static_cast<BasicBlock *>(valueMap[&callee_B]);
                            if (!dynamic_cast<const Ret *>(&callee_I)) {
                                std::shared_ptr<Instruction> caller_I = cast<Instruction>(callee_I.clone());
                                if (dynamic_cast<const Alloca *>(&*caller_I)) {
                                    F.entry().prepend(caller_I);
                                } else {
                                    caller_B->append(caller_I);
                                }
                                valueMap.put(&callee_I, &*caller_I);
                                cloned.push_back(&*caller_I);
                            } else {
                                caller_B->append(std::make_shared<Br>(weaken(*B)));
                                if (*callee->functionType()->returnType() != Void()) {
                                    exits.emplace_back(caller_B, &*static_cast<const Ret *>(&callee_I)->value());
                                }
                            }
                        }
                    }

                    for (Instruction *caller_I : cloned) {
                        for (UseBase *op : caller_I->operands()) {
                            if (auto k = valueMap.find(&**op); k != valueMap.end()) {
                                op->set(share(*k->second));
                            }
                        }
                    }

                    if (*callee->functionType()->returnType() != Void()) {
                        for (auto [caller_B, value] : exits) {
                            if (auto k = valueMap.find(value); k != valueMap.end()) {
                                value = k->second;
                            }
                            static_cast<Phi *>(&B->front())->addIncoming(weaken(*caller_B), share(*value));
                        }
                    }

                    removeFromParent(*call);
                    changed = true;

                    break;
                }
            }
        }
    }

    assert(F.isWellFormed());
    return changed;
}
