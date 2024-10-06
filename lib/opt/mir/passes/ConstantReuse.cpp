#include "mini-llvm/opt/mir/passes/ConstantReuse.h"

#include <algorithm>
#include <cstddef>
#include <list>
#include <unordered_map>
#include <utility>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/IntegerImmediate.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/mir/StackRelativeOffsetImmediate.h"
#include "mini-llvm/utils/Hash.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

struct ImmediateHash {
    size_t operator()(const Immediate *imm) const noexcept {
        if (auto *castImm = dynamic_cast<const IntegerImmediate *>(&*imm)) {
            return hash_value(castImm->value());
        }
        if (auto *castImm = dynamic_cast<const StackRelativeOffsetImmediate *>(&*imm)) {
            size_t seed = 0;

            hash_combine(seed, castImm->slot1());
            hash_combine(seed, castImm->slot2());
            hash_combine(seed, castImm->displacement());

            return seed;
        }
        std::unreachable();
    }
};

struct ImmediateEqual {
    bool operator()(const Immediate *lhs, const Immediate *rhs) const noexcept {
        if (auto *castLhs = dynamic_cast<const IntegerImmediate *>(&*lhs)) {
            if (auto *castRhs = dynamic_cast<const IntegerImmediate *>(&*rhs)) {
                return castLhs->value() == castRhs->value();
            }
        }
        if (auto *castLhs = dynamic_cast<const StackRelativeOffsetImmediate *>(&*lhs)) {
            if (auto *castRhs = dynamic_cast<const StackRelativeOffsetImmediate *>(&*rhs)) {
                return castLhs->slot1() == castRhs->slot1()
                    && castLhs->slot2() == castRhs->slot2()
                    && castLhs->displacement() == castRhs->displacement();
            }
        }
        return false;
    }
};

} // namespace

bool ConstantReuse::runOnBasicBlock(BasicBlock &B) {
    bool changed = false;

    std::unordered_map<Register *, const Immediate *> imms;
    std::unordered_map<const Immediate *, std::list<Register *>, ImmediateHash, ImmediateEqual> regLists;

    for (Instruction &I : B) {
        for (Register *reg : def(I)) {
            if (imms.contains(reg)) {
                const Immediate *imm = imms[reg];
                imms.erase(reg);
                regLists[imm].erase(std::ranges::find(regLists[imm], reg));
            }
        }
        if (auto *li = dynamic_cast<const LI *>(&I)) {
            imms[&*li->dst()] = &*li->src();
            regLists[&*li->src()].push_back(&*li->dst());
        }
        for (RegisterOperand *op : I.srcs()) {
            if (imms.contains(&**op)) {
                Register *reg = regLists[imms[&**op]].front();
                if (&**op != reg) {
                    op->set(share(*reg));
                    changed = true;
                }
            }
        }
    }

    return changed;
}
