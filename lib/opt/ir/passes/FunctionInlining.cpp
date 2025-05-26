#include "mini-llvm/opt/ir/passes/FunctionInlining.h"

#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Attribute.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
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

constexpr size_t kThreshold = 20;

bool isShort(const Function &F) {
    size_t count = 0;
    for (const BasicBlock &B : F) {
        count += B.size();
    }
    return count <= kThreshold;
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

bool shouldInlineHeuristic(const Call &call) {
    return isShort(*call.callee()) && !isRecursive(*call.callee());
}

bool shouldInline(const Call &call) {
    if (call.callee()->empty()) return false;
    if (call.callee()->hasAttr(Attribute::kNoInline)) return false;
    if (call.callee()->hasAttr(Attribute::kAlwaysInline)) return true;
    return shouldInlineHeuristic(call);
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
                    phi->putIncoming(*B2, std::move(value));
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

    for (BasicBlock &B : F) {
        for (BasicBlock::const_iterator i = B.begin(), e = B.end(); i != e; ++i) {
            if (auto *call = dynamic_cast<const Call *>(&*i)) {
                const Function *callee = &*call->callee();
                if (shouldInline(*call)) {
                    BasicBlock *B2 = splitBefore(std::next(i));

                    if (*callee->functionType()->returnType() != Void()) {
                        Phi &phi = B2->prepend(std::make_shared<Phi>(callee->functionType()->returnType()));
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
                                caller_B->append(caller_I);
                                valueMap.put(&callee_I, &*caller_I);
                                cloned.push_back(&*caller_I);
                            } else {
                                caller_B->append(std::make_shared<Br>(weaken(*B2)));
                                if (*callee->functionType()->returnType() != Void()) {
                                    exits.emplace_back(caller_B, &*static_cast<const Ret *>(&callee_I)->value());
                                }
                            }
                        }
                    }

                    for (Instruction *caller_I : cloned) {
                        for (UseBase *op : caller_I->operands()) {
                            if (auto j = valueMap.find(&**op); j != valueMap.end()) {
                                op->set(share(*j->second));
                            }
                        }
                    }

                    if (*callee->functionType()->returnType() != Void()) {
                        for (auto [caller_B, value] : exits) {
                            if (auto j = valueMap.find(value); j != valueMap.end()) {
                                value = j->second;
                            }
                            static_cast<Phi *>(&B2->front())->putIncoming(*caller_B, share(*value));
                        }
                    }

                    removeFromParent(*call);
                    changed = true;

                    break;
                }
            }
        }
    }

    return changed;
}
