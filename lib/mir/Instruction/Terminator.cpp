// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Instruction/Terminator.h"

#include <unordered_set>
#include <utility>

using namespace mini_llvm::mir;

std::unordered_set<BasicBlockOperand *> Terminator::blockOps() {
    std::unordered_set<BasicBlockOperand *> blockOps;
    for (const BasicBlockOperand *op : std::as_const(*this).blockOps()) {
        blockOps.insert(const_cast<BasicBlockOperand *>(op));
    }
    return blockOps;
}
