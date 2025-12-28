// SPDX-License-Identifier: MIT

#pragma once

#include <unordered_set>

#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT Terminator : public Instruction {
public:
    bool hasSideEffects() const override {
        return true;
    }

    virtual std::unordered_set<const BasicBlockOperand *> blockOps() const = 0;
    std::unordered_set<BasicBlockOperand *> blockOps();
};

} // namespace mini_llvm::mir
