// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/mc/RISCVInstruction.h"

#include <format>
#include <string>

#include "mini-llvm/targets/riscv/mc/RISCVOperation.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mc;

std::string RISCVInstruction::format() const {
    StringJoiner formatted(" ");
    formatted.add(mnemonic(static_cast<RISCVOperation>(opcode())));
    if (!operand_empty()) {
        StringJoiner formattedOperands(", ");
        for (const Operand &operand : operands(*this)) {
            formattedOperands.add("{}", operand);
        }
        formatted.add("{}", formattedOperands);
    }
    return formatted.toString();
}
