#include "mini-llvm/targets/riscv/opt/mir/passes/RISCVConstantPropagation.h"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Add.h"
#include "mini-llvm/mir/Instruction/AddI.h"
#include "mini-llvm/mir/Instruction/And.h"
#include "mini-llvm/mir/Instruction/AndI.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Mov.h"
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
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

bool RISCVConstantPropagation::runOnBasicBlock(BasicBlock &B) {
    std::unordered_map<Register *, int64_t> values;
    std::vector<std::pair<BasicBlock::const_iterator, std::unique_ptr<Instruction>>> replace;

    for (BasicBlock::const_iterator i = B.begin(), e = B.end(); i != e; ++i) {
        if (auto *li = dynamic_cast<const LI *>(&*i)) {
            if (auto *imm = dynamic_cast<const IntegerImmediate *>(&*li->src())) {
                values[&*li->dst()] = imm->value();
            }
        }
        if (auto *mov = dynamic_cast<const Mov *>(&*i)) {
            if (auto j = values.find(&*mov->src()); j != values.end()) {
                int64_t value = j->second;
                std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                replace.emplace_back(i, std::make_unique<LI>(mov->width(), share(*mov->dst()), std::move(imm)));
            }
        }
        if (auto *add = dynamic_cast<const Add *>(&*i)) {
            if (auto j = values.find(&*add->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= value && value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                    replace.emplace_back(i, std::make_unique<AddI>(add->width(), share(*add->dst()), share(*add->src1()), std::move(imm), add->extensionMode()));
                }
            }
        }
        if (auto *sub = dynamic_cast<const Sub *>(&*i)) {
            if (auto j = values.find(&*sub->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= -value && -value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(-value);
                    replace.emplace_back(i, std::make_unique<AddI>(sub->width(), share(*sub->dst()), share(*sub->src1()), std::move(imm), sub->extensionMode()));
                }
            }
        }
        if (auto *and_ = dynamic_cast<const And *>(&*i)) {
            if (auto j = values.find(&*and_->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= value && value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                    replace.emplace_back(i, std::make_unique<AndI>(and_->width(), share(*and_->dst()), share(*and_->src1()), std::move(imm), and_->extensionMode()));
                }
            }
        }
        if (auto *or_ = dynamic_cast<const Or *>(&*i)) {
            if (auto j = values.find(&*or_->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= value && value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                    replace.emplace_back(i, std::make_unique<OrI>(or_->width(), share(*or_->dst()), share(*or_->src1()), std::move(imm), or_->extensionMode()));
                }
            }
        }
        if (auto *xor_ = dynamic_cast<const Xor *>(&*i)) {
            if (auto j = values.find(&*xor_->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= value && value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                    replace.emplace_back(i, std::make_unique<XorI>(xor_->width(), share(*xor_->dst()), share(*xor_->src1()), std::move(imm), xor_->extensionMode()));
                }
            }
        }
        if (auto *shl = dynamic_cast<const SHL *>(&*i)) {
            if (auto j = values.find(&*shl->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= value && value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                    replace.emplace_back(i, std::make_unique<SHLI>(shl->width(), share(*shl->dst()), share(*shl->src1()), std::move(imm), shl->extensionMode()));
                }
            }
        }
        if (auto *shrl = dynamic_cast<const SHRL *>(&*i)) {
            if (auto j = values.find(&*shrl->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= value && value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                    replace.emplace_back(i, std::make_unique<SHRLI>(shrl->width(), share(*shrl->dst()), share(*shrl->src1()), std::move(imm), shrl->extensionMode()));
                }
            }
        }
        if (auto *shra = dynamic_cast<const SHRA *>(&*i)) {
            if (auto j = values.find(&*shra->src2()); j != values.end()) {
                int64_t value = j->second;
                if (-2048 <= value && value <= 2047) {
                    std::unique_ptr<Immediate> imm = std::make_unique<IntegerImmediate>(value);
                    replace.emplace_back(i, std::make_unique<SHRAI>(shra->width(), share(*shra->dst()), share(*shra->src1()), std::move(imm), shra->extensionMode()));
                }
            }
        }
        if (!dynamic_cast<const LI *>(&*i)) {
            for (Register *reg : def(*i)) {
                values.erase(reg);
            }
        }
    }

    bool changed = false;

    for (auto &[i, I] : replace) {
        B.add(i, std::move(I));
        B.remove(i);
        changed = true;
    }

    return changed;
}
