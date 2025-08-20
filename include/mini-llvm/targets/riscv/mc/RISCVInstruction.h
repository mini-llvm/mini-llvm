// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT RISCVInstruction final : public Instruction {
public:
    explicit RISCVInstruction(int opcode, std::vector<std::unique_ptr<Operand>> operands = {})
        : Instruction(opcode, std::move(operands)) {}

    std::string format() const override;
};

} // namespace mini_llvm::mc
