// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/BasicBlock.h"

#include <cassert>
#include <cstdint>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Terminator.h"
#include "mini-llvm/utils/SetOps.h"
#include "mini-llvm/utils/StringJoiner.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;
using namespace mini_llvm::set_ops;

Instruction &BasicBlock::add(BasicBlock::const_iterator pos, std::unique_ptr<Instruction> I) {
    return **insts_.insert(pos.base(), std::move(I));
}

std::unique_ptr<Instruction> BasicBlock::remove(BasicBlock::const_iterator pos) {
    std::unique_ptr<Instruction> I = std::move(const_cast<std::unique_ptr<Instruction> &>(*pos.base()));
    insts_.erase(pos.base());
    return I;
}

void BasicBlock::replace(BasicBlock::const_iterator pos, std::unique_ptr<Instruction> I) {
    const_cast<std::unique_ptr<Instruction> &>(*pos.base()) = std::move(I);
}

void BasicBlock::clear() {
    while (!empty()) {
        remove(begin());
    }
}

std::string BasicBlock::format() const {
    StringJoiner formatted("\n");
    formatted.add("{}:", formatAsOperand());
    for (const Instruction &I : *this) {
        formatted.add("  {}", I);
    }
    return formatted.toString();
}

std::string BasicBlock::formatAsOperand() const {
    if (name().empty()) {
        return "#_" + toString(reinterpret_cast<uintptr_t>(this), 62);
    }
    return "#" + name();
}

std::unordered_set<BasicBlock *> mir::successors(const BasicBlock &B) {
    assert(!B.empty() && dynamic_cast<const Terminator *>(&B.back()));
    std::unordered_set<BasicBlock *> successors;
    for (const BasicBlockOperand *op : static_cast<const Terminator &>(B.back()).blockOps()) {
        successors.insert(&**op);
    }
    return successors;
}

std::unordered_set<Register *> mir::use(const BasicBlock &B) {
    std::unordered_set<Register *> Use;
    for (const Instruction &I : std::views::reverse(B)) {
        Use = (Use - def(I)) | use(I);
    }
    return Use;
}

std::unordered_set<Register *> mir::def(const BasicBlock &B) {
    std::unordered_set<Register *> Def;
    for (const Instruction &I : B) {
        Def |= def(I);
    }
    return Def;
}
