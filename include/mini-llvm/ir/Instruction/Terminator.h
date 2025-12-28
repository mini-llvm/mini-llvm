// SPDX-License-Identifier: MIT

#pragma once

#include <unordered_set>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Terminator : public Instruction {
public:
    virtual std::unordered_set<BasicBlock *> successors() const = 0;
};

} // namespace mini_llvm::ir
