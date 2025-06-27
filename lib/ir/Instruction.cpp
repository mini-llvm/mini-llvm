#include "mini-llvm/ir/Instruction.h"

#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Use.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::unordered_set<UseBase *> Instruction::operands() {
    std::unordered_set<UseBase *> operands;
    for (const UseBase *operand : std::as_const(*this).operands()) {
        operands.insert(const_cast<UseBase *>(operand));
    }
    return operands;
}

Instruction &ir::addToParent(const Instruction &before, std::shared_ptr<Instruction> I) {
    return before.parent()->add(before.parentIterator(), std::move(I));
}

void ir::removeFromParent(const Instruction &I) {
    I.parent()->remove(I.parentIterator());
}
