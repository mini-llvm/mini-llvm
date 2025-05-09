#include "mini-llvm/ir/BasicBlock.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/utils/StringJoiner.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

Instruction &BasicBlock::add(BasicBlock::const_iterator pos, std::shared_ptr<Instruction> I) {
    assert(I->parent_ == nullptr);
    assert(I->parentIterator_ == std::nullopt);
    iterator newPos(insts_.insert(pos.base(), std::move(I)));
    newPos->parent_ = this;
    newPos->parentIterator_ = newPos;
    return *newPos;
}

void BasicBlock::remove(BasicBlock::const_iterator pos) {
    pos->parent_ = nullptr;
    pos->parentIterator_ = std::nullopt;
    insts_.erase(pos.base());
}

std::string BasicBlock::format() const {
    StringJoiner formatted("\n");
    formatted.add(formatAsLabel());
    for (const Instruction &I : *this) {
        formatted.addFormat("  {}", I);
    }
    return formatted.toString();
}

std::string BasicBlock::formatAsOperand() const {
    if (name().empty()) {
        return std::format("%_{}", toString(reinterpret_cast<uintptr_t>(this), 62));
    }
    if (!std::ranges::all_of(name(), [](char ch) { return isalnum(ch) || ch == '_' || ch == '.'; })) {
        return std::format("%{}", quote(name()));
    }
    return std::format("%{}", name());
}

std::string BasicBlock::formatAsLabel() const {
    if (name().empty()) {
        return std::format("_{}:", toString(reinterpret_cast<uintptr_t>(this), 62));
    }
    if (!std::ranges::all_of(name(), [](char ch) { return isalnum(ch) || ch == '_' || ch == '.'; })) {
        return std::format("{}:", quote(name()));
    }
    return std::format("{}:", name());
}

bool ir::hasNPredecessors(const BasicBlock &B, size_t n) {
    size_t count = 0;
    for (const UseBase &use : uses(B)) {
        if (dynamic_cast<const Terminator *>(use.user())) {
            ++count;
            if (count > n) {
                return false;
            }
        }
    }
    if (count < n) {
        return false;
    }
    return true;
}

bool ir::hasNPredecessorsOrMore(const BasicBlock &B, size_t n) {
    size_t count = 0;
    for (const UseBase &use : uses(B)) {
        if (dynamic_cast<const Terminator *>(use.user())) {
            ++count;
            if (count == n) {
                return true;
            }
        }
    }
    return false;
}

std::unordered_set<BasicBlock *> ir::predecessors(const BasicBlock &B) {
    std::unordered_set<BasicBlock *> predecessors;
    for (const UseBase &use : uses(B)) {
        if (auto *terminator = dynamic_cast<const Terminator *>(use.user())) {
            predecessors.insert(terminator->parent());
        }
    }
    return predecessors;
}

std::unordered_set<BasicBlock *> ir::successors(const BasicBlock &B) {
    return dynamic_cast<const Terminator &>(B.back()).successors();
}

void ir::removeFromParent(const BasicBlock &B) {
    B.parent()->remove(B.parentIterator());
}
