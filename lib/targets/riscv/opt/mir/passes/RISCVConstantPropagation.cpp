// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/opt/mir/passes/RISCVConstantPropagation.h"

#include <bit>
#include <cstdint>
#include <memory>
#include <utility>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Add.h"
#include "mini-llvm/mir/Instruction/AddI.h"
#include "mini-llvm/mir/Instruction/And.h"
#include "mini-llvm/mir/Instruction/AndI.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Mov.h"
#include "mini-llvm/mir/Instruction/Mul.h"
#include "mini-llvm/mir/Instruction/Or.h"
#include "mini-llvm/mir/Instruction/OrI.h"
#include "mini-llvm/mir/Instruction/SHL.h"
#include "mini-llvm/mir/Instruction/SHLI.h"
#include "mini-llvm/mir/Instruction/SHRA.h"
#include "mini-llvm/mir/Instruction/SHRAI.h"
#include "mini-llvm/mir/Instruction/SHRL.h"
#include "mini-llvm/mir/Instruction/SHRLI.h"
#include "mini-llvm/mir/Instruction/Sub.h"
#include "mini-llvm/mir/Instruction/Xor.h"
#include "mini-llvm/mir/Instruction/XorI.h"
#include "mini-llvm/mir/IntegerImmediate.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

void propagate(BasicBlock &B, BasicBlock::const_iterator i, const HashMap<Register *, int64_t> &values, bool &changed) {
    if (auto *mov = dynamic_cast<const Mov *>(&*i)) {
        if (auto j = values.find(&*mov->src()); j != values.end()) {
            int64_t value = j->second;
            std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
            std::unique_ptr<Instruction> I = std::make_unique<LI>(mov->width(), share(*mov->dst()), std::move(imm));
            B.replace(i, std::move(I));
            changed = true;
            return;
        }
        return;
    }
    if (auto *add = dynamic_cast<const Add *>(&*i)) {
        if (auto j = values.find(&*add->src1()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<AddI>(
                    add->width(), share(*add->dst()), share(*add->src2()), std::move(imm), add->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        if (auto j = values.find(&*add->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<AddI>(
                    add->width(), share(*add->dst()), share(*add->src1()), std::move(imm), add->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *sub = dynamic_cast<const Sub *>(&*i)) {
        if (auto j = values.find(&*sub->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= -value && -value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(-value);
                std::unique_ptr<Instruction> I = std::make_unique<AddI>(
                    sub->width(), share(*sub->dst()), share(*sub->src1()), std::move(imm), sub->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *and_ = dynamic_cast<const And *>(&*i)) {
        if (auto j = values.find(&*and_->src1()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<AndI>(
                    and_->width(), share(*and_->dst()), share(*and_->src2()), std::move(imm), and_->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        if (auto j = values.find(&*and_->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<AndI>(
                    and_->width(), share(*and_->dst()), share(*and_->src1()), std::move(imm), and_->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *or_ = dynamic_cast<const Or *>(&*i)) {
        if (auto j = values.find(&*or_->src1()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<OrI>(
                    or_->width(), share(*or_->dst()), share(*or_->src2()), std::move(imm), or_->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        if (auto j = values.find(&*or_->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<OrI>(
                    or_->width(), share(*or_->dst()), share(*or_->src1()), std::move(imm), or_->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *xor_ = dynamic_cast<const Xor *>(&*i)) {
        if (auto j = values.find(&*xor_->src1()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<XorI>(
                    xor_->width(), share(*xor_->dst()), share(*xor_->src2()), std::move(imm), xor_->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        if (auto j = values.find(&*xor_->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<XorI>(
                    xor_->width(), share(*xor_->dst()), share(*xor_->src1()), std::move(imm), xor_->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *shl = dynamic_cast<const SHL *>(&*i)) {
        if (auto j = values.find(&*shl->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<SHLI>(
                    shl->width(), share(*shl->dst()), share(*shl->src1()), std::move(imm), shl->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *shrl = dynamic_cast<const SHRL *>(&*i)) {
        if (auto j = values.find(&*shrl->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<SHRLI>(
                    shrl->width(), share(*shrl->dst()), share(*shrl->src1()), std::move(imm), shrl->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *shra = dynamic_cast<const SHRA *>(&*i)) {
        if (auto j = values.find(&*shra->src2()); j != values.end()) {
            int64_t value = j->second;
            if (-2048 <= value && value <= 2047) {
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                std::unique_ptr<Instruction> I = std::make_unique<SHRAI>(
                    shra->width(), share(*shra->dst()), share(*shra->src1()), std::move(imm), shra->extMode()
                );
                B.replace(i, std::move(I));
                changed = true;
                return;
            }
            return;
        }
        return;
    }
    if (auto *mul = dynamic_cast<const Mul *>(&*i)) {
        if (mul->width() == 8 || mul->width() == 4) {
            if (auto j = values.find(&*mul->src1()); j != values.end()) {
                int64_t value = j->second;
                if (std::has_single_bit((uint64_t)value)) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(std::countr_zero((uint64_t)value));
                    std::unique_ptr<Instruction> I = std::make_unique<SHLI>(
                        mul->width(), share(*mul->dst()), share(*mul->src2()), std::move(imm), mul->extMode()
                    );
                    B.replace(i, std::move(I));
                    changed = true;
                    return;
                }
                return;
            }
            if (auto j = values.find(&*mul->src2()); j != values.end()) {
                int64_t value = j->second;
                if (std::has_single_bit((uint64_t)value)) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(std::countr_zero((uint64_t)value));
                    std::unique_ptr<Instruction> I = std::make_unique<SHLI>(
                        mul->width(), share(*mul->dst()), share(*mul->src1()), std::move(imm), mul->extMode()
                    );
                    B.replace(i, std::move(I));
                    changed = true;
                    return;
                }
                return;
            }
            return;
        }
        return;
    }
}

} // namespace

bool RISCVConstantPropagation::runOnBasicBlock(BasicBlock &B) {
    bool changed = false;

    HashMap<Register *, int64_t> values;

    for (BasicBlock::const_iterator i = B.begin(), e = B.end(); i != e; ++i) {
        if (auto *li = dynamic_cast<const LI *>(&*i)) {
            if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*li->src())) {
                values.put(&*li->dst(), imm->value());
                continue;
            }
        }

        propagate(B, i, values, changed);

        for (Register *reg : def(*i)) {
            values.erase(reg);
        }
    }

    HashMap<Register *, std::pair<Register *, int64_t>> sums;

    for (Instruction &I : B) {
        if (auto *addi = dynamic_cast<const AddI *>(&I)) {
            if (&*addi->dst() != &*addi->src1()) {
                if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*addi->src2())) {
                    sums.put(&*addi->dst(), {&*addi->src1(), imm->value()});
                    continue;
                }
            }
        }
        for (MemoryOperand *op : I.memOps()) {
            if (auto i = sums.find(&*op->baseReg()); i != sums.end()) {
                if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*op->offset())) {
                    auto [newBaseReg, newOffset] = i->second;
                    newOffset += imm->value();
                    if (-2048 <= newOffset && newOffset <= 2047) {
                        op->baseReg().set(share(*newBaseReg));
                        op->offset().set(std::make_unique<IntegerImmediate>(newOffset));
                        changed = true;
                    }
                }
            }
        }
        for (Register *reg : def(I)) {
            sums.erase(reg);
            for (auto i = sums.begin(); i != sums.end();) {
                if (i->second.first == reg) {
                    i = sums.erase(i);
                } else {
                    ++i;
                }
            }
        }
    }

    return changed;
}
