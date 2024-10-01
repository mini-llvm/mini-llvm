#pragma once

#include <unordered_set>

#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Instruction.h"

namespace mini_llvm::mir {

class Terminator : public Instruction {
public:
    bool hasSideEffects() const override {
        return true;
    }

    virtual std::unordered_set<const BasicBlockOperand *> successors() const = 0;
    std::unordered_set<BasicBlockOperand *> successors();
};

} // namespace mini_llvm::mir
