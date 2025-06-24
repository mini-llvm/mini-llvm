#include "mini-llvm/ir/Instruction.h"

#include <algorithm>
#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Use.h"
#include "mini-llvm/utils/Chars.h"
#include "mini-llvm/utils/Strings.h"

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

std::string Instruction::formatAsOperand() const {
    if (name().empty()) {
        return std::format("%_{}", toString(reinterpret_cast<uintptr_t>(this), 62));
    }
    if (!std::ranges::all_of(name(), [](char ch) { return isLetterOrDigit(ch) || ch == '_' || ch == '.'; })) {
        return std::format("%{}", quote(name()));
    }
    return std::format("%{}", name());
}

void ir::removeFromParent(const Instruction &I) {
    I.parent()->remove(I.parentIterator());
}
