#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/targets/riscv/mc/RISCVOperation.h"

namespace mini_llvm::mc {

class RISCVInstruction final : public Instruction {
public:
    explicit RISCVInstruction(int opcode, std::vector<std::unique_ptr<Operand>> operands = {})
        : Instruction(opcode, std::move(operands)) {}

protected:
    std::string mnemonic(int opcode) const override {
        return mc::mnemonic(static_cast<RISCVOperation>(opcode));
    }
};

} // namespace mini_llvm::mc
