// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/mir/Instruction/RISCVCall.h"

#include <format>
#include <ranges>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::unordered_set<PhysicalRegister *> RISCVCall::implicitDsts() const {
    return riscvCallImplicitDsts();
}

std::unordered_set<PhysicalRegister *> RISCVCall::implicitSrcs() const {
    return riscvCallImplicitSrcs(numIntegerArgs(), numFloatingArgs());
}

std::string RISCVCall::format() const {
    StringJoiner formattedRegs(", ");
    for (PhysicalRegister *physReg : std::views::take(riscvIntegerArgRegs(), numIntegerArgs())) {
        formattedRegs.add("{}", *physReg);
    }
    for (PhysicalRegister *physReg : std::views::take(riscvFloatingArgRegs(), numFloatingArgs())) {
        formattedRegs.add("{}", *physReg);
    }
    return std::format("CALL {:o}, {}", *callee(), formattedRegs);
}
