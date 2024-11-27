#include "mini-llvm/targets/riscv/mir/Instruction/RISCVCall.h"

#include <ranges>
#include <unordered_set>

#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

using namespace mini_llvm::mir;
using namespace mini_llvm::mir::riscv;

std::unordered_set<PhysicalRegister *> RISCVCall::implicitDsts() const {
    std::unordered_set<PhysicalRegister *> implicitDsts;
    for (PhysicalRegister *physReg : riscvRegs()) {
        if (!physReg->isPreserved()) {
            implicitDsts.insert(physReg);
        }
    }
    return implicitDsts;
}

std::unordered_set<PhysicalRegister *> RISCVCall::implicitSrcs() const {
    std::unordered_set<PhysicalRegister *> implicitSrcs{ra(), sp(), gp(), tp(), fp()};
    for (PhysicalRegister *physReg : std::views::take(riscvIntegerArgRegs(), numIntegerArgs())) {
        implicitSrcs.insert(physReg);
    }
    for (PhysicalRegister *physReg : std::views::take(riscvFloatingArgRegs(), numFloatingArgs())) {
        implicitSrcs.insert(physReg);
    }
    return implicitSrcs;
}
