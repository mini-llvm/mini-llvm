// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/NullOperationElimination.h"

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Instruction/AddI.h"
#include "mini-llvm/mir/Instruction/AndI.h"
#include "mini-llvm/mir/Instruction/FMov.h"
#include "mini-llvm/mir/Instruction/Mov.h"
#include "mini-llvm/mir/Instruction/OrI.h"
#include "mini-llvm/mir/Instruction/SHLI.h"
#include "mini-llvm/mir/Instruction/SHRAI.h"
#include "mini-llvm/mir/Instruction/SHRLI.h"
#include "mini-llvm/mir/Instruction/XorI.h"
#include "mini-llvm/mir/IntegerImmediate.h"
#include "mini-llvm/mir/RegisterOperand.h"

using namespace mini_llvm::mir;

bool NullOperationElimination::runOnBasicBlock(BasicBlock &B) {
    bool changed = false;

    for (auto i = B.begin(); i != B.end();) {
        if (auto *mov = dynamic_cast<const Mov *>(&*i)) {
            if (mov->width() == mov->dst()->width()) {
                if (&*mov->dst() == &*mov->src()) {
                    B.remove(i++);
                    changed = true;
                    continue;
                }
            }
        }
        if (auto *fmov = dynamic_cast<const FMov *>(&*i)) {
            if (&*fmov->dst() == &*fmov->src()) {
                B.remove(i++);
                changed = true;
                continue;
            }
        }
        if (auto *addi = dynamic_cast<const AddI *>(&*i)) {
            if (addi->width() == addi->dst()->width()) {
                if (&*addi->dst() == &*addi->src1()) {
                    if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*addi->src2())) {
                        if (imm->value() == 0) {
                            B.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        if (auto *andi = dynamic_cast<const AndI *>(&*i)) {
            if (andi->width() == andi->dst()->width()) {
                if (&*andi->dst() == &*andi->src1()) {
                    if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*andi->src2())) {
                        if (imm->value() == -1) {
                            B.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        if (auto *ori = dynamic_cast<const OrI *>(&*i)) {
            if (ori->width() == ori->dst()->width()) {
                if (&*ori->dst() == &*ori->src1()) {
                    if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*ori->src2())) {
                        if (imm->value() == 0) {
                            B.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        if (auto *xori = dynamic_cast<const XorI *>(&*i)) {
            if (xori->width() == xori->dst()->width()) {
                if (&*xori->dst() == &*xori->src1()) {
                    if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*xori->src2())) {
                        if (imm->value() == 0) {
                            B.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        if (auto *shli = dynamic_cast<const SHLI *>(&*i)) {
            if (shli->width() == shli->dst()->width()) {
                if (&*shli->dst() == &*shli->src1()) {
                    if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*shli->src2())) {
                        if (imm->value() == 0) {
                            B.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        if (auto *shrli = dynamic_cast<const SHRLI *>(&*i)) {
            if (shrli->width() == shrli->dst()->width()) {
                if (&*shrli->dst() == &*shrli->src1()) {
                    if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*shrli->src2())) {
                        if (imm->value() == 0) {
                            B.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        if (auto *shrai = dynamic_cast<const SHRAI *>(&*i)) {
            if (shrai->width() == shrai->dst()->width()) {
                if (&*shrai->dst() == &*shrai->src1()) {
                    if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*shrai->src2())) {
                        if (imm->value() == 0) {
                            B.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        ++i;
    }

    return changed;
}
