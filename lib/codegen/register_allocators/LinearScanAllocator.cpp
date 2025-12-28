// SPDX-License-Identifier: MIT

#include "mini-llvm/codegen/register_allocators/LinearScanAllocator.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <set>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"
#include "mini-llvm/opt/mir/passes/LiveVariableAnalysis.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/SetOps.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;
using namespace mini_llvm::set_ops;

// Massimiliano Poletto and Vivek Sarkar. 1999. Linear scan register allocation.
// ACM Trans. Program. Lang. Syst. 21, 5 (Sept. 1999), 895-913.
// https://doi.org/10.1145/330249.330250

namespace {

struct Interval {
    VirtualRegister *virtReg;
    size_t start, end;
};

bool compareStart(const Interval &lhs, const Interval &rhs) {
    if (lhs.start != rhs.start) return lhs.start < rhs.start;
    if (lhs.end != rhs.end) return lhs.end < rhs.end;
    return lhs.virtReg < rhs.virtReg;
}

bool compareEnd(const Interval &lhs, const Interval &rhs) {
    if (lhs.end != rhs.end) return lhs.end < rhs.end;
    if (lhs.start != rhs.start) return lhs.start < rhs.start;
    return lhs.virtReg < rhs.virtReg;
}

struct CompareEnd {
    bool operator()(const Interval *lhs, const Interval *rhs) const {
        return compareEnd(*lhs, *rhs);
    }
};

class Linearize {
public:
    explicit Linearize(const Function *function) : function_(function) {}

    std::vector<const Instruction *> operator()() {
        for (const BasicBlock &B : *function_) {
            if (!visited_.contains(&B)) {
                visit(B);
            }
        }
        std::vector<const Instruction *> seq;
        for (const BasicBlock *B : std::views::reverse(seq_)) {
            for (const Instruction &I : *B) {
                seq.push_back(&I);
            }
        }
        return seq;
    }

private:
    const Function *function_;
    std::unordered_set<const BasicBlock *> visited_;
    std::vector<const BasicBlock *> seq_;

    void visit(const BasicBlock &B) {
        visited_.insert(&B);
        for (const BasicBlock *successor : successors(B)) {
            if (!visited_.contains(successor)) {
                visit(*successor);
            }
        }
        seq_.push_back(&B);
    }
};

bool isBetter(PhysicalRegister *lhs, PhysicalRegister *rhs) {
    if (lhs->isPreserved() != rhs->isPreserved()) {
        return lhs->isPreserved() < rhs->isPreserved();
    }
    if (lhs->idx() != rhs->idx()) {
        return lhs->idx() < rhs->idx();
    }
    return false;
}

bool isBetter(PhysicalRegister *lhs, PhysicalRegister *rhs, const std::unordered_set<PhysicalRegister *> &preferred) {
    bool preferLHS = preferred.contains(lhs),
         preferRHS = preferred.contains(rhs);
    if (preferLHS != preferRHS) {
        return preferLHS;
    }
    return isBetter(lhs, rhs);
}

} // namespace

bool LinearScanAllocator::allocate(
    Function &F,
    const std::unordered_set<VirtualRegister *> &virtRegs,
    const std::unordered_set<PhysicalRegister *> &physRegs,
    RegisterAction load,
    RegisterAction store
) {
#ifndef NDEBUG
    for (PhysicalRegister *physReg : physRegs) {
        assert(physReg->isAllocatable());
    }
#endif

    LiveVariableAnalysis liveVars;
    liveVars.runOnFunction(F);

    size_t numReserved = 0;
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            std::unordered_set<VirtualRegister *> srcs, dsts;
            for (const RegisterOperand *op : I.srcs()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && virtRegs.contains(virtReg)) {
                    srcs.insert(virtReg);
                }
            }
            for (const RegisterOperand *op : I.dsts()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && virtRegs.contains(virtReg)) {
                    dsts.insert(virtReg);
                }
            }
            numReserved = std::max({numReserved, srcs.size(), dsts.size()});
        }
    }
    std::unordered_set<PhysicalRegister *> reserved(physRegs.begin(), physRegs.end());
    for (Register *reg : liveVars.liveIn(F.entry())) {
        if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg); physReg && physRegs.contains(physReg)) {
            reserved.erase(physReg);
        }
    }
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            for (Register *reg : def(I)) {
                if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg); physReg && physRegs.contains(physReg)) {
                    reserved.erase(physReg);
                }
            }
        }
    }
    while (reserved.size() > numReserved) {
        PhysicalRegister *bestPhysReg = nullptr;
        for (PhysicalRegister *physReg : reserved) {
            if (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg)) {
                bestPhysReg = physReg;
            }
        }
        reserved.erase(bestPhysReg);
    }
    if (reserved.size() < numReserved) {
        return false;
    }

    std::vector<const Instruction *> seq = Linearize(&F)();

    HashMap<VirtualRegister *, std::pair<size_t, size_t>> endpoints;
    for (auto [i, I] : std::views::enumerate(seq)) {
        for (Register *reg : liveVars.liveOut(*I) | def(*I)) {
            if (auto *virtReg = dynamic_cast<VirtualRegister *>(reg); virtReg && virtRegs.contains(virtReg)) {
                if (!endpoints.contains(virtReg)) {
                    endpoints.put(virtReg, {i, i});
                } else {
                    endpoints[virtReg].first = std::min(endpoints[virtReg].first, (size_t)i);
                    endpoints[virtReg].second = std::max(endpoints[virtReg].second, (size_t)i);
                }
            }
        }
    }

    std::vector<Interval> intervals;
    for (VirtualRegister *virtReg : virtRegs) {
        auto [start, end] = endpoints[virtReg];
        intervals.emplace_back(virtReg, start, end);
    }

    HashMap<VirtualRegister *, std::unordered_set<PhysicalRegister *>> allocatable;
    for (VirtualRegister *virtReg : virtRegs) {
        allocatable.put(virtReg, physRegs - reserved);
        auto [start, end] = endpoints[virtReg];
        for (Register *reg : liveVars.liveOut(*seq[start])) {
            if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg); physReg && physRegs.contains(physReg)) {
                allocatable[virtReg].erase(physReg);
            }
        }
        for (size_t i = start; i <= end; ++i) {
            for (Register *reg : def(*seq[i])) {
                if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg); physReg && physRegs.contains(physReg)) {
                    allocatable[virtReg].erase(physReg);
                }
            }
        }
    }

    HashMap<VirtualRegister *, std::unordered_set<Register *>> coalescence;
    for (VirtualRegister *virtReg : virtRegs) {
        coalescence.put(virtReg, {});
    }
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            if (I.isCoalescent()) {
                Register *dst = I.coalescenceDst(),
                         *src = I.coalescenceSrc();
                if (auto *virtRegDst = dynamic_cast<VirtualRegister *>(dst); virtRegDst && virtRegs.contains(virtRegDst)) {
                    if (auto *virtRegSrc = dynamic_cast<VirtualRegister *>(src); virtRegSrc && virtRegs.contains(virtRegSrc)) {
                        coalescence[virtRegDst].insert(virtRegSrc);
                    }
                    if (auto *physRegSrc = dynamic_cast<PhysicalRegister *>(src); physRegSrc && physRegs.contains(physRegSrc)) {
                        coalescence[virtRegDst].insert(physRegSrc);
                    }
                }
                if (auto *virtRegSrc = dynamic_cast<VirtualRegister *>(src); virtRegSrc && virtRegs.contains(virtRegSrc)) {
                    if (auto *virtRegDst = dynamic_cast<VirtualRegister *>(dst); virtRegDst && virtRegs.contains(virtRegDst)) {
                        coalescence[virtRegSrc].insert(virtRegDst);
                    }
                    if (auto *physRegDst = dynamic_cast<PhysicalRegister *>(dst); physRegDst && physRegs.contains(physRegDst)) {
                        coalescence[virtRegSrc].insert(physRegDst);
                    }
                }
            }
        }
    }

    std::unordered_set<PhysicalRegister *> free = physRegs - reserved;
    std::set<const Interval *, CompareEnd> active;
    HashMap<VirtualRegister *, PhysicalRegister *> allocation;
    std::unordered_set<VirtualRegister *> spilled;

    std::ranges::sort(intervals, compareStart);

    for (const Interval &I : intervals) {
        for (auto i = active.begin(); i != active.end();) {
            if ((*i)->end >= I.start) break;
            free.insert(allocation[(*i)->virtReg]);
            i = active.erase(i);
        }
        PhysicalRegister *bestPhysReg = nullptr;
        std::unordered_set<PhysicalRegister *> preferred;
        for (Register *reg : coalescence[I.virtReg]) {
            if (auto *physReg = dynamic_cast<PhysicalRegister *>(reg)) {
                preferred.insert(physReg);
            }
            if (auto *virtReg = dynamic_cast<VirtualRegister *>(reg); virtReg && allocation.contains(virtReg)) {
                preferred.insert(allocation[virtReg]);
            }
        }
        for (PhysicalRegister *physReg : free) {
            if (allocatable[I.virtReg].contains(physReg) && (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg, preferred))) {
                bestPhysReg = physReg;
            }
        }
        if (bestPhysReg != nullptr) {
            free.erase(bestPhysReg);
            allocation.put(I.virtReg, bestPhysReg);
            active.insert(&I);
        } else {
            spilled.insert(I.virtReg);
        }
    }

    for (BasicBlock &B : F) {
        for (Instruction &I : B) {
            for (RegisterOperand *op : I.regOps()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && allocation.contains(virtReg)) {
                    op->set(share(*allocation[virtReg]));
                }
            }
        }
    }

    HashMap<VirtualRegister *, StackSlot *> slots;
    for (VirtualRegister *virtReg : spilled) {
        slots.put(virtReg, &F.stackFrame().add(std::prev(F.stackFrame().end()), virtReg->width(), virtReg->width()));
    }

    for (BasicBlock &B : F) {
        std::vector<std::unique_ptr<Instruction>> tmp;
        while (!B.empty()) {
            tmp.push_back(B.removeFirst());
        }
        BasicBlockBuilder builder(&B);
        for (const auto &I : tmp) {
            std::unordered_set<VirtualRegister *> srcs, dsts;
            for (const RegisterOperand *op : I->srcs()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && slots.contains(virtReg)) {
                    srcs.insert(virtReg);
                }
            }
            for (const RegisterOperand *op : I->dsts()) {
                if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && slots.contains(virtReg)) {
                    dsts.insert(virtReg);
                }
            }
            HashMap<VirtualRegister *, PhysicalRegister *> allocation;
            std::unordered_set<PhysicalRegister *> srcAllocated, dstAllocated;
            for (VirtualRegister *virtReg : srcs & dsts) {
                PhysicalRegister *bestPhysReg = nullptr;
                for (PhysicalRegister *physReg : reserved) {
                    if (!srcAllocated.contains(physReg) && !dstAllocated.contains(physReg) &&
                            (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg))) {
                        bestPhysReg = physReg;
                    }
                }
                allocation.put(virtReg, bestPhysReg);
                srcAllocated.insert(bestPhysReg);
                dstAllocated.insert(bestPhysReg);
            }
            for (VirtualRegister *virtReg : srcs - dsts) {
                PhysicalRegister *bestPhysReg = nullptr;
                for (PhysicalRegister *physReg : reserved) {
                    if (!srcAllocated.contains(physReg) && (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg))) {
                        bestPhysReg = physReg;
                    }
                }
                allocation.put(virtReg, bestPhysReg);
                srcAllocated.insert(bestPhysReg);
            }
            for (VirtualRegister *virtReg : dsts - srcs) {
                PhysicalRegister *bestPhysReg = nullptr;
                for (PhysicalRegister *physReg : reserved) {
                    if (!dstAllocated.contains(physReg) && (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg))) {
                        bestPhysReg = physReg;
                    }
                }
                allocation.put(virtReg, bestPhysReg);
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

    return true;
}
