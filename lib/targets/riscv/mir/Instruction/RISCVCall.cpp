#include "mini-llvm/targets/riscv/mir/Instruction/RISCVCall.h"

#include <unordered_set>

#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

using namespace mini_llvm::mir;
using namespace mini_llvm::mir::riscv;

std::unordered_set<PhysicalRegister *> RISCVCall::implicitDsts() const {
    return riscvCallImplicitDsts();
}

std::unordered_set<PhysicalRegister *> RISCVCall::implicitSrcs() const {
    return riscvCallImplicitSrcs(numIntegerArgs(), numFloatingArgs());
}
