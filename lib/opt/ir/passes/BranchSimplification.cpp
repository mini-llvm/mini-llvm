// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/BranchSimplification.h"

#include <cassert>
#include <memory>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Br.h"
#include "mini-llvm/ir/Instruction/CondBr.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

bool BranchSimplification::runOnFunction(Function &F) {
    bool changed = false;

    for (const BasicBlock &B : F) {
        if (auto *condBr = dynamic_cast<const CondBr *>(&B.back())) {
            if (auto *cond = dynamic_cast<const I1Constant *>(&*condBr->cond())) {
                BasicBlock *dest, *notDest;
                if (cond->value() == true) {
                    dest = &*condBr->trueDest();
                    notDest = &*condBr->falseDest();
                } else {
                    dest = &*condBr->falseDest();
                    notDest = &*condBr->trueDest();
                }
                addToParent(*condBr, std::make_shared<Br>(weaken(*dest)));
                removeFromParent(*condBr);
                changed = true;
                for (Instruction &I : *notDest) {
                    if (auto *phi = dynamic_cast<Phi *>(&I)) {
                        if (hasIncomingBlock(*phi, B)) {
                            removeIncomingBlock(*phi, B);
                            changed = true;
                        }
                    }
                }
                continue;
            }

            if (&*condBr->trueDest() == &*condBr->falseDest()) {
                BasicBlock *dest = &*condBr->trueDest();
                addToParent(*condBr, std::make_shared<Br>(weaken(*dest)));
                removeFromParent(*condBr);
                changed = true;
                continue;
            }
        }
    }

    assert(F.isWellFormed());
    return changed;
}
