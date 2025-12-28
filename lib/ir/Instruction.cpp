// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction.h"

#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::unordered_set<UseBase *> Instruction::operands() {
    std::unordered_set<UseBase *> operands;
    for (const UseBase *operand : std::as_const(*this).operands()) {
        operands.insert(const_cast<UseBase *>(operand));
    }
    return operands;
}

bool Instruction::isWellFormed() const {
    if (!Value::isWellFormed()) {
        return false;
    }
    for (const UseBase *op : operands()) {
        if (op->expired()) {
            return false;
        }
    }
    return true;
}

Instruction &ir::addToParent(const Instruction &before, std::shared_ptr<Instruction> I) {
    return before.parent()->add(before.parentIterator(), std::move(I));
}

void ir::removeFromParent(const Instruction &I) {
    I.parent()->remove(I.parentIterator());
}
