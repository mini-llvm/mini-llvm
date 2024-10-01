#include "mini-llvm/ir/BasicBlock.h"

#include <cassert>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <unordered_set>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

Instruction &BasicBlock::add(BasicBlock::const_iterator pos, std::shared_ptr<Instruction> I) {
    assert(I->parent_ == nullptr);
    assert(I->parentIterator_ == std::nullopt);
    iterator pos2 = iterator(insts_.insert(pos.base(), std::move(I)));
    pos2->parent_ = this;
    pos2->parentIterator_ = pos2;
    return *pos2;
}

void BasicBlock::remove(BasicBlock::const_iterator pos) {
    pos->parent_ = nullptr;
    pos->parentIterator_ = std::nullopt;
    insts_.erase(pos.base());
}

std::string BasicBlock::format() const {
    StringJoiner formatted("\n");
    formatted.addFormat("{}:", formatAsOperand().substr(1));
    for (const Instruction &I : *this) {
        formatted.addFormat("  {}", I);
    }
    return formatted.toString();
}

bool mini_llvm::ir::hasNPredecessors(const BasicBlock &B, size_t n) {
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

bool mini_llvm::ir::hasNPredecessorsOrMore(const BasicBlock &B, size_t n) {
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

bool mini_llvm::ir::hasPredecessor(const BasicBlock &B, const BasicBlock &pred) {
    for (const UseBase &use : uses(B)) {
        if (auto *terminator = dynamic_cast<const Terminator *>(use.user())) {
            if (terminator->parent() == &pred) {
                return true;
            }
        }
    }
    return false;
}

std::unordered_set<BasicBlock *> mini_llvm::ir::predecessors(const BasicBlock &B) {
    std::unordered_set<BasicBlock *> predecessors;
    for (const UseBase &use : uses(B)) {
        if (auto *terminator = dynamic_cast<const Terminator *>(use.user())) {
            predecessors.insert(terminator->parent());
        }
    }
    return predecessors;
}

std::unordered_set<BasicBlock *> mini_llvm::ir::successors(const BasicBlock &B) {
    return dynamic_cast<const Terminator &>(B.back()).successors();
}

void mini_llvm::ir::removeFromParent(const BasicBlock &B) {
    B.parent()->remove(B.parentIterator());
}
