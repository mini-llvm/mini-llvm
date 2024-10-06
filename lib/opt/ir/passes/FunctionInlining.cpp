#include "mini-llvm/opt/ir/passes/FunctionInlining.h"

#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <unordered_map>
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

bool shouldInlineHeuristic(const Function &F) {
    return isShort(F) && !isRecursive(F);
}

bool shouldInline(const Function &F) {
    if (F.empty()) return false;
    if (F.hasAttr(Attribute::kNoInline)) return false;
    if (F.hasAttr(Attribute::kAlwaysInline)) return true;
    return shouldInlineHeuristic(F);
}

BasicBlock *splitBefore(BasicBlock::const_iterator i) {
    BasicBlock *B = i->parent();
    Function *F = B->parent();

    BasicBlock *B2 = &F->append();

    for (BasicBlock *succ : successors(*B)) {
        for (Instruction &I : *succ) {
            if (auto *phi = dynamic_cast<Phi *>(&I)) {
                if (auto i = phi->findIncoming(*B); i != phi->incoming_end()) {
                    std::shared_ptr<Value> value = share(*i->value);
                    phi->removeIncoming(i);
                    phi->putIncoming(*B2, std::move(value));
                }
            }
        }
    }

    std::vector<std::shared_ptr<Instruction>> taken;

    for (auto j = i; j != B->end(); ++j) {
        taken.push_back(share(*const_cast<Instruction *>(&*j)));
    }
    for (auto &I : taken) {
        removeFromParent(*I);
    }
    for (auto &I : taken) {
        B2->append(std::move(I));
    }

    return B2;
}

} // namespace

bool FunctionInlining::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        for (BasicBlock::const_iterator i = B.begin(); i != B.end(); ++i) {
            if (auto *call = dynamic_cast<const Call *>(&*i)) {
                const Function *callee = &*call->callee();
                if (shouldInline(*callee)) {
                    BasicBlock *B2 = splitBefore(std::next(i));

                    if (*callee->functionType()->returnType() != Void()) {
                        Phi &phi = B2->prepend(std::make_shared<Phi>(callee->functionType()->returnType()));
                        replaceAllUsesWith(*call, weaken(phi));
                    }

                    std::unordered_map<const Value *, Value *> valueMap;

                    for (const BasicBlock &callee_B : *callee) {
                        valueMap[&callee_B] = &F.append();
                    }

                    for (auto [arg1, arg2] : std::views::zip(args(*callee), args(*call))) {
                        valueMap[&arg1] = &*arg2;
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
                                valueMap[&callee_I] = &*caller_I;
                                cloned.push_back(&*caller_I);
                            } else {
                                caller_B->append(std::make_shared<Br>(weaken(*B2)));
                                if (*callee->functionType()->returnType() != Void()) {
                                    exits.push_back({caller_B, &*static_cast<const Ret *>(&callee_I)->value()});
                                }
                            }
                        }
                    }

                    for (Instruction *caller_I : cloned) {
                        for (UseBase *op : caller_I->operands()) {
                            if (auto i = valueMap.find(&**op); i != valueMap.end()) {
                                op->set(share(*i->second));
                            }
                        }
                    }

                    if (*callee->functionType()->returnType() != Void()) {
                        for (auto [caller_B, value] : exits) {
                            if (auto i = valueMap.find(value); i != valueMap.end()) {
                                value = i->second;
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
