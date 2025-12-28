// SPDX-License-Identifier: MIT

#include "mini-llvm/codegen/register_allocators/GraphColoringAllocator.h"

#include <cassert>
#include <iterator>
#include <memory>
#include <stack>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
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

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

bool hasEdge(HashMap<Register *, std::unordered_set<Register *>> &g, Register *u, Register *v) {
    return g[u].contains(v);
}

void addEdge(HashMap<Register *, std::unordered_set<Register *>> &g, Register *u, Register *v) {
    g[u].insert(v);
    g[v].insert(u);
}

void removeEdge(HashMap<Register *, std::unordered_set<Register *>> &g, Register *u, Register *v) {
    g[u].erase(v);
    g[v].erase(u);
}

bool isBetter(PhysicalRegister *lhs, PhysicalRegister *rhs) {
    if (lhs->isPreserved() != rhs->isPreserved()) {
        return lhs->isPreserved() < rhs->isPreserved();
    }
    if (lhs->idx() != rhs->idx()) {
        return lhs->idx() < rhs->idx();
    }
    return false;
}

} // namespace

bool GraphColoringAllocator::allocate(
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

    std::unordered_set<Register *> regs;
    for (VirtualRegister *virtReg : virtRegs) {
        regs.insert(virtReg);
    }
    for (PhysicalRegister *physReg : physRegs) {
        regs.insert(physReg);
    }

    for (;;) {
        LiveVariableAnalysis liveVars;
        liveVars.runOnFunction(F);

        HashMap<Register *, std::unordered_set<Register *>> interference;

        for (Register *v : regs) {
            interference.put(v, {});
        }

        for (const BasicBlock &B : F) {
            std::unordered_set<Register *> liveIn;
            for (Register *reg : liveVars.liveIn(B)) {
                if (regs.contains(reg)) {
                    liveIn.insert(reg);
                }
            }
            for (Register *u : liveIn) {
                for (Register *v : liveIn) {
                    if (u != v) {
                        addEdge(interference, u, v);
                    }
                }
            }
            for (const Instruction &I : B) {
                std::unordered_set<Register *> liveOut;
                for (Register *reg : liveVars.liveOut(I)) {
                    if (regs.contains(reg)) {
                        liveOut.insert(reg);
                    }
                }
                std::unordered_set<Register *> Def;
                for (Register *reg : def(I)) {
                    if (regs.contains(reg)) {
                        Def.insert(reg);
                    }
                }
                for (Register *u : liveOut) {
                    for (Register *v : liveOut) {
                        if (u != v) {
                            addEdge(interference, u, v);
                        }
                    }
                }
                for (Register *u : liveOut) {
                    for (Register *v : Def) {
                        if (u != v) {
                            addEdge(interference, u, v);
                        }
                    }
                }
            }
        }

        HashMap<Register *, std::unordered_set<Register *>> coalescence;

        for (Register *v : regs) {
            coalescence.put(v, {});
        }

        for (const BasicBlock &B : F) {
            for (const Instruction &I : B) {
                if (I.isCoalescent()) {
                    Register *u = I.coalescenceDst(),
                             *v = I.coalescenceSrc();
                    if (u != v && regs.contains(u) && regs.contains(v) && !hasEdge(interference, u, v)) {
                        addEdge(coalescence, u, v);
                    }
                }
            }
        }

        for (const auto &[u, N] : coalescence) {
            if (!dynamic_cast<VirtualRegister *>(u)) {
                continue;
            }
            for (Register *v : N) {
                std::unordered_set<Register *> S;
                for (Register *w : interference[u]) {
                    if (w != v && interference[w].size() >= physRegs.size()) {
                        S.insert(w);
                    }
                }
                for (Register *w : interference[v]) {
                    if (w != u && interference[w].size() >= physRegs.size()) {
                        S.insert(w);
                    }
                }

                if (S.size() < physRegs.size()) {
                    for (BasicBlock &B : F) {
                        for (Instruction &I : B) {
                            for (RegisterOperand *op : I.regOps()) {
                                if (&**op == u) {
                                    op->set(share(*v));
                                }
                            }
                        }
                    }

                    regs.erase(u);

                    for (Register *w : std::unordered_set(interference[u])) {
                        if (w != v) {
                            addEdge(interference, v, w);
                            removeEdge(coalescence, v, w);
                        }
                    }
                    for (Register *w : std::unordered_set(interference[u])) {
                        removeEdge(interference, u, w);
                    }

                    for (Register *w : std::unordered_set(coalescence[u])) {
                        if (w != v && !hasEdge(interference, v, w)) {
                            addEdge(coalescence, v, w);
                        }
                    }
                    for (Register *w : std::unordered_set(coalescence[u])) {
                        removeEdge(coalescence, u, w);
                    }

                    break;
                }
            }
        }

        HashMap<Register *, int> degrees;

        for (const auto &[u, N] : interference) {
            degrees.put(u, N.size());
        }

        std::unordered_set<Register *> V = regs;
        std::stack<Register *> S;
        HashMap<VirtualRegister *, PhysicalRegister *> allocation;
        std::unordered_set<VirtualRegister *> spilled;

        while (!V.empty()) {
            Register *u = nullptr;
            for (Register *v : V) {
                if (u == nullptr || degrees[v] < degrees[u]) {
                    u = v;
                }
            }
            V.erase(u);
            for (Register *v : interference[u]) {
                --degrees[v];
            }
            S.push(u);
        }

        while (!S.empty()) {
            Register *u = S.top();
            S.pop();
            if (auto *virtReg = dynamic_cast<VirtualRegister *>(u)) {
                std::unordered_set<PhysicalRegister *> used;
                for (Register *v : interference[u]) {
                    if (auto *otherPhysReg = dynamic_cast<PhysicalRegister *>(v)) {
                        used.insert(otherPhysReg);
                    }
                    if (auto *otherVirtReg = dynamic_cast<VirtualRegister *>(v)) {
                        if (auto i = allocation.find(otherVirtReg); i != allocation.end()) {
                            used.insert(i->second);
                        }
                    }
                }
                PhysicalRegister *bestPhysReg = nullptr;
                for (PhysicalRegister *physReg : physRegs) {
                    if (!used.contains(physReg) && (bestPhysReg == nullptr || isBetter(physReg, bestPhysReg))) {
                        bestPhysReg = physReg;
                    }
                }
                if (bestPhysReg != nullptr) {
                    allocation.put(virtReg, bestPhysReg);
                } else {
                    spilled.insert(virtReg);
                }
            }
        }

        for (auto i = regs.begin(); i != regs.end();) {
            if (dynamic_cast<VirtualRegister *>(*i)) {
                i = regs.erase(i);
                continue;
            }
            ++i;
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

        if (spilled.empty()) {
            return true;
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
                HashMap<VirtualRegister *, std::shared_ptr<VirtualRegister>> srcs, dsts;
                for (const RegisterOperand *op : I->srcs()) {
                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && slots.contains(virtReg)) {
                        std::shared_ptr<VirtualRegister> newVirtReg = std::make_shared<VirtualRegister>(virtReg->width());
                        regs.insert(&*newVirtReg);
                        srcs.put(virtReg, std::move(newVirtReg));
                    }
                }
                for (const RegisterOperand *op : I->dsts()) {
                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && slots.contains(virtReg)) {
                        std::shared_ptr<VirtualRegister> newVirtReg = std::make_shared<VirtualRegister>(virtReg->width());
                        regs.insert(&*newVirtReg);
                        dsts.put(virtReg, std::move(newVirtReg));
                    }
                }
                for (const auto &[virtReg, newVirtReg] : srcs) {
                    load(&*newVirtReg, slots[virtReg], builder);
                }
                std::unique_ptr<Instruction> II = I->clone();
                for (RegisterOperand *op : II->srcs()) {
                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && srcs.contains(virtReg)) {
                        op->set(srcs[virtReg]);
                    }
                }
                for (RegisterOperand *op : II->dsts()) {
                    if (auto *virtReg = dynamic_cast<VirtualRegister *>(&**op); virtReg && dsts.contains(virtReg)) {
                        op->set(dsts[virtReg]);
                    }
                }
                builder.add(std::move(II));
                for (const auto &[virtReg, newVirtReg] : dsts) {
                    store(&*newVirtReg, slots[virtReg], builder);
                }
            }
        }
    }
}
