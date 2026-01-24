// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/RegisterReuse.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/IntegerImmediate.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/mir/StackOffsetImmediate.h"
#include "mini-llvm/utils/HashCombine.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

struct ImmediateHash {
    size_t operator()(const Immediate *imm) const noexcept {
        if (auto *integerImm = dynamic_cast<const IntegerImmediate *>(&*imm)) {
            return std::hash<int64_t>()(integerImm->value());
        }
        if (auto *stackOffsetImm = dynamic_cast<const StackOffsetImmediate *>(&*imm)) {
            size_t seed = 0;

            hash_combine(seed, stackOffsetImm->baseSlot());
            hash_combine(seed, stackOffsetImm->slot());
            hash_combine(seed, stackOffsetImm->displacement());

            return seed;
        }
        return 0;
    }
};

struct ImmediateEqual {
    bool operator()(const Immediate *lhs, const Immediate *rhs) const noexcept {
        if (auto *lhsInteger = dynamic_cast<const IntegerImmediate *>(&*lhs)) {
            if (auto *rhsInteger = dynamic_cast<const IntegerImmediate *>(&*rhs)) {
                return lhsInteger->value() == rhsInteger->value();
            }
        }
        if (auto *lhsStackOffset = dynamic_cast<const StackOffsetImmediate *>(&*lhs)) {
            if (auto *rhsStackOffset = dynamic_cast<const StackOffsetImmediate *>(&*rhs)) {
                return lhsStackOffset->baseSlot() == rhsStackOffset->baseSlot()
                    && lhsStackOffset->slot() == rhsStackOffset->slot()
                    && lhsStackOffset->displacement() == rhsStackOffset->displacement();
            }
        }
        return false;
    }
};

} // namespace

bool RegisterReuse::runOnBasicBlock(BasicBlock &B) {
    bool changed = false;

    HashMap<Register *, const Immediate *> imms;
    HashMap<const Immediate *, std::list<Register *>, ImmediateHash, ImmediateEqual> regs;

    for (Instruction &I : B) {
        for (Register *reg : def(I)) {
            if (imms.contains(reg)) {
                const Immediate *imm = imms[reg];
                imms.erase(reg);
                regs[imm].erase(std::ranges::find(regs[imm], reg));
            }
        }
        if (auto *li = dynamic_cast<const LI *>(&I)) {
            imms.put(&*li->dst(), &*li->src());
            if (!regs.contains(&*li->src())) {
                regs.put(&*li->src(), {});
            }
            regs[&*li->src()].push_back(&*li->dst());
        }
        for (RegisterOperand *op : I.srcs()) {
            if (imms.contains(&**op)) {
                Register *reg = regs[imms[&**op]].front();
                if (&**op != reg) {
                    op->set(share(*reg));
                    changed = true;
                }
            }
        }
    }

    return changed;
}
