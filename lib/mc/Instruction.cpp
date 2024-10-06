#include "mini-llvm/mc/Instruction.h"

#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mc;

std::string Instruction::format() const {
    StringJoiner formatted(" ");
    formatted.add(mnemonic(opcode()));
    if (!operand_empty()) {
        StringJoiner formattedOperands(", ");
        for (const Operand &operand : operands(*this)) {
            formattedOperands.addFormat("{}", operand);
        }
        formatted.addFormat("{}", formattedOperands);
    }
    return formatted.toString();
}
