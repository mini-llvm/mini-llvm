// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/JumpThreading.h"

#include <cassert>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

bool JumpThreading::runOnFunction(Function &F) {
    bool changed = false;

    bool changed2;
    do {
        changed2 = false;

        for (BasicBlock &B : F) {
            if (B.size() == 1) {
                if (auto *br = dynamic_cast<Br *>(&B.back())) {
                    BasicBlock *succ = &*br->dest();
                    if (succ != &B) {
                        for (BasicBlock *pred : predecessors(B)) {
                            if (!successors(*pred).contains(succ)) {
                                for (Instruction &I : *succ) {
                                    if (auto *phi = dynamic_cast<Phi *>(&I)) {
                                        phi->addIncoming(weaken(*pred), share(*getIncomingValue(*phi, B)));
                                        changed2 = true;
                                    }
                                }
                                for (UseBase *op : pred->back().operands()) {
                                    if (&**op == &B) {
                                        op->set(weaken(*succ));
                                        changed2 = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        changed |= changed2;
    } while (changed2);

    assert(F.isWellFormed());
    return changed;
}
