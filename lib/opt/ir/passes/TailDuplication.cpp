// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/TailDuplication.h"

#include <cassert>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

namespace {

bool isDuplicable(const BasicBlock &B) {
    for (const Instruction &I : B) {
        if (!I.use_empty()) {
            return false;
        }
    }
    return true;
}

} // namespace

bool TailDuplication::runOnFunction(Function &F) {
    bool changed = false;

    bool changed2;
    do {
        changed2 = false;

        HashMap<BasicBlock *, std::vector<BasicBlock *>> predecessors;
        for (BasicBlock &B : F) {
            predecessors.put(&B, {});
        }
        for (BasicBlock &B : F) {
            for (BasicBlock *succ : successors(B)) {
                predecessors[succ].push_back(&B);
            }
        }

        for (BasicBlock &B : F) {
            if (B.size() <= threshold_ && !successors(B).contains(&B) && isDuplicable(B)) {
                for (BasicBlock *pred : predecessors[&B]) {
                    if (dynamic_cast<Br *>(&pred->back())) {
                        pred->removeLast();
                        for (Instruction &I : B) {
                            pred->append(cast<Instruction>(I.clone()));
                        }
                        for (Instruction &I : B) {
                            if (auto *phi = dynamic_cast<Phi *>(&I)) {
                                removeIncomingBlock(*phi, *pred);
                            } else {
                                break;
                            }
                        }
                        for (BasicBlock *succ : successors(B)) {
                            for (Instruction &I : *succ) {
                                if (auto *phi = dynamic_cast<Phi *>(&I)) {
                                    phi->addIncoming(weaken(*pred), share(*getIncomingValue(*phi, B)));
                                } else {
                                    break;
                                }
                            }
                        }
                        changed2 = true;
                    }
                }
            }
        }

        changed |= changed2;
    } while (changed2);

    assert(F.isWellFormed());
    return changed;
}
