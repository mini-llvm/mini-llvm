#include "mini-llvm/codegen/register_allocator/NaiveAllocator.h"

#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/mir/StackFrame.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"
#include "mini-llvm/opt/mir/passes/LiveVariableAnalysis.h"
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/SetOps.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;
using namespace mini_llvm::set_ops;

namespace {

bool isBetter(PhysicalRegister *lhs, PhysicalRegister *rhs) {
    if (lhs->isVolatile() != rhs->isVolatile()) {
        return lhs->isVolatile() > rhs->isVolatile();
    }
    if (lhs->idx() != rhs->idx()) {
        return lhs->idx() < rhs->idx();
    }
    return false;
}

} // namespace

void NaiveAllocator::allocate(Function &F,
                              int regWidth,
                              const std::unordered_set<mir::VirtualRegister *> &virtRegs,
                              const std::unordered_set<mir::PhysicalRegister *> &physRegs,
                              std::function<void (PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder)> load,
                              std::function<void (PhysicalRegister *physReg, StackSlot *slot, const BasicBlockBuilder &builder)> store) {
#ifndef NDEBUG
    for (PhysicalRegister *physReg : physRegs) {
        assert(physReg->isAllocatable());
    }
#endif

    std::unordered_map<VirtualRegister *, StackSlot *> slots;
    for (VirtualRegister *virtReg : virtRegs) {
        slots[virtReg] = &F.stackFrame().add(std::prev(F.stackFrame().end()), regWidth, regWidth);
    }

    LiveVariableAnalysis liveVars;
    liveVars.runOnFunction(F);

    for (BasicBlock &B : F) {
        std::vector<std::unique_ptr<Instruction>> taken;
        while (!B.empty()) {
            taken.push_back(B.removeFirst());
        }
        BasicBlockBuilder builder(&B);
        for (const auto &I : taken) {
            std::unordered_set<VirtualRegister *> srcs, dsts;
            for (const RegisterOperand *op : I->srcs()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op)) {
                    srcs.insert(virtReg);
                }
            }
            for (const RegisterOperand *op : I->dsts()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op)) {
                    dsts.insert(virtReg);
                }
            }
            std::unordered_set<PhysicalRegister *> liveIn, liveOut;
            for (Register *reg : liveVars.liveIn(*I)) {
                if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg); physReg && physRegs.contains(physReg)) {
                    liveIn.insert(physReg);
                }
            }
            for (Register *reg : liveVars.liveOut(*I)) {
                if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg); physReg && physRegs.contains(physReg)) {
                    liveOut.insert(physReg);
                }
            }
            std::unordered_map<VirtualRegister *, PhysicalRegister *> allocation;
            std::unordered_set<PhysicalRegister *> srcAllocated, dstAllocated;
            for (VirtualRegister *virtReg : srcs & dsts) {
                std::unordered_set<PhysicalRegister *> allocatable = physRegs - liveIn - liveOut;
                PhysicalRegister *bestPhysReg = nullptr;
                for (PhysicalRegister *physReg : allocatable) {
                    if (!srcAllocated.contains(physReg) && !dstAllocated.contains(physReg) &&
                            (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg))) {
                        bestPhysReg = physReg;
                    }
                }
                if (bestPhysReg == nullptr) {
                    throw RegisterAllocationException();
                }
                allocation[virtReg] = bestPhysReg;
                srcAllocated.insert(bestPhysReg);
                dstAllocated.insert(bestPhysReg);
            }
            for (VirtualRegister *virtReg : srcs - dsts) {
                std::unordered_set<PhysicalRegister *> allocatable = physRegs - liveIn;
                PhysicalRegister *bestPhysReg = nullptr;
                for (PhysicalRegister *physReg : allocatable) {
                    if (!srcAllocated.contains(physReg) && (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg))) {
                        bestPhysReg = physReg;
                    }
                }
                if (bestPhysReg == nullptr) {
                    throw RegisterAllocationException();
                }
                allocation[virtReg] = bestPhysReg;
                srcAllocated.insert(bestPhysReg);
            }
            for (VirtualRegister *virtReg : dsts - srcs) {
                std::unordered_set<PhysicalRegister *> allocatable = physRegs - liveOut;
                PhysicalRegister *bestPhysReg = nullptr;
                for (PhysicalRegister *physReg : allocatable) {
                    if (!dstAllocated.contains(physReg) && (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg))) {
                        bestPhysReg = physReg;
                    }
                }
                if (bestPhysReg == nullptr) {
                    throw RegisterAllocationException();
                }
                allocation[virtReg] = bestPhysReg;
                dstAllocated.insert(bestPhysReg);
            }
            for (VirtualRegister *virtReg : srcs) {
                load(allocation[virtReg], slots[virtReg], builder);
            }
            std::unique_ptr<Instruction> II = I->clone();
            for (RegisterOperand *op : II->regOps()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && allocation.contains(virtReg)) {
                    op->set(share(*allocation[virtReg]));
                }
            }
            builder.add(std::move(II));
            for (VirtualRegister *virtReg : dsts) {
                store(allocation[virtReg], slots[virtReg], builder);
            }
        }
    }
}
