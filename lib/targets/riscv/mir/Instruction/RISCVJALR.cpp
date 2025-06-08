#include "mini-llvm/targets/riscv/mir/Instruction/RISCVJALR.h"

#include <unordered_set>

#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

using namespace mini_llvm::mir;
using namespace mini_llvm::mir::riscv;

std::unordered_set<PhysicalRegister *> RISCVJALR::implicitDsts() const {
    return riscvCallImplicitDsts();
}

std::unordered_set<PhysicalRegister *> RISCVJALR::implicitSrcs() const {
    return riscvCallImplicitSrcs(numIntegerArgs(), numFloatingArgs());
}
