// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Function.h"

#include <cassert>
#include <iterator>
#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/Attribute.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Ret.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/Dot.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

Function::Function(std::unique_ptr<FunctionType> functionType, Linkage linkage)
        : functionType_(std::move(functionType)), linkage_(linkage) {
    for (Type &paramType : paramTypes(*functionType_)) {
        args_.push_back(std::make_shared<Argument>(paramType.clone()));
    }
}

BasicBlock &Function::add(Function::const_iterator pos, std::shared_ptr<BasicBlock> block) {
    assert(block->parent_ == nullptr);
    assert(block->parentIterator_ == std::nullopt);
    iterator newPos(blocks_.insert(pos.base(), std::move(block)));
    newPos->parent_ = this;
    newPos->parentIterator_ = newPos;
    return *newPos;
}

BasicBlock &Function::add(Function::const_iterator pos) {
    return add(pos, std::make_shared<BasicBlock>());
}

BasicBlock &Function::prepend(std::shared_ptr<BasicBlock> block) {
    return add(begin(), std::move(block));
}

BasicBlock &Function::prepend() {
    return prepend(std::make_shared<BasicBlock>());
}

BasicBlock &Function::append(std::shared_ptr<BasicBlock> block) {
    return add(end(), std::move(block));
}

BasicBlock &Function::append() {
    return append(std::make_shared<BasicBlock>());
}

void Function::remove(Function::const_iterator pos) {
    pos->parent_ = nullptr;
    pos->parentIterator_ = std::nullopt;
    blocks_.erase(pos.base());
}

void Function::removeFirst() {
    remove(begin());
}

void Function::removeLast() {
    remove(std::prev(end()));
}

void Function::clear() {
    blocks_.clear();
}

bool Function::isWellFormed() const {
    if (!GlobalValue::isWellFormed()) {
        return false;
    }
    if (isDeclaration()) {
        return true;
    }
    for (const BasicBlock &B : *this) {
        if (!B.isWellFormed()) {
            return false;
        }
    }
    for (const BasicBlock &B : *this) {
        for (const Instruction &I : B) {
            if (auto *ret = dynamic_cast<const Ret *>(&I)) {
                if (*ret->value()->type() != *functionType()->returnType()) {
                    return false;
                }
            }
        }
    }
    if (!hasNPredecessors(entry(), 0)) {
        return false;
    }
    for (const BasicBlock &B : *this) {
        for (const Instruction &I : B) {
            if (auto *phi = dynamic_cast<const Phi *>(&I)) {
                std::unordered_set<BasicBlock *> incomingBlocks;
                for (Phi::ConstIncoming incoming : incomings(*phi)) {
                    if (!incomingBlocks.insert(&*incoming.block).second) {
                        return false;
                    }
                }
                if (incomingBlocks != predecessors(B)) {
                    return false;
                }
            }
        }
    }
    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(*this);
    std::unordered_set<const BasicBlock *> visited;
    std::queue<const BasicBlock *> Q;
    visited.insert(&entry());
    Q.push(&entry());
    while (!Q.empty()) {
        const BasicBlock *u = Q.front();
        Q.pop();
        for (const BasicBlock *v : successors(*u)) {
            if (visited.insert(v).second) {
                Q.push(v);
            }
        }
    }
    for (const BasicBlock *B : visited) {
        for (const Instruction &I : *B) {
            for (const UseBase &use : uses(I)) {
                if (auto *II = dynamic_cast<const Instruction *>(use.user())) {
                    if (!dynamic_cast<const Phi *>(II) && visited.contains(II->parent()) && !domTree.dominates(I, *II)) {
                        return false;
                    }
                }
            }
        }
    }
    for (const BasicBlock &B : *this) {
        if (!visited.contains(&B)) {
            for (const Instruction &I : B) {
                for (const UseBase &use : uses(I)) {
                    if (auto *II = dynamic_cast<const Instruction *>(use.user())) {
                        if (!dynamic_cast<const Phi *>(II) && visited.contains(II->parent())) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    for (const BasicBlock *B : visited) {
        for (const Instruction &I : *B) {
            if (auto *phi = dynamic_cast<const Phi *>(&I)) {
                for (Phi::ConstIncoming incoming : incomings(*phi)) {
                    if (visited.contains(&*incoming.block)) {
                        if (auto *II = dynamic_cast<const Instruction *>(&*incoming.value)) {
                            if (!visited.contains(II->parent())) {
                                return false;
                            }
                            if (!domTree.dominates(*II->parent(), *incoming.block)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    for (const BasicBlock &B : *this) {
        if (&B != &entry()) {
            for (const Instruction &I : B) {
                if (dynamic_cast<const Alloca *>(&I)) {
                    return false;
                }
            }
        }
    }
    return true;
}

std::string Function::format() const {
    StringJoiner formatted(" ");
    if (!isDeclaration()) {
        formatted.add("define");
    } else {
        formatted.add("declare");
    }
    switch (linkage()) {
    case Linkage::kInternal:
        formatted.add("internal");
        break;
    case Linkage::kPrivate:
        formatted.add("private");
        break;
    case Linkage::kExternal:
        break;
    }
    StringJoiner formattedArgs(", ", "(", ")");
    for (const Argument &arg : args(*this)) {
        if (isDeclaration()) {
            formattedArgs.add("{}", *arg.type());
        } else {
            formattedArgs.add("{} {:o}", *arg.type(), arg);
        }
    }
    if (functionType()->isVarArgs()) {
        formattedArgs.add("...");
    }
    formatted.add("{} {:o}{}", *functionType()->returnType(), *this, formattedArgs);
    for (const Attribute &attr : attrs(*this)) {
        formatted.add("{}", attr);
    }
    if (!isDeclaration()) {
        StringJoiner formattedBody("\n\n", "{\n", "\n}");
        for (const BasicBlock &B : *this) {
            formattedBody.add("{}", B);
        }
        formatted.add("{}", formattedBody);
    }
    return formatted.toString();
}

std::string ir::toDot(const Function &F) {
    std::vector<std::pair<std::string, std::string>> edges;
    for (const BasicBlock &B : F) {
        for (const BasicBlock *succ : successors(B)) {
            edges.emplace_back(B.formatAsOperand(), succ->formatAsOperand());
        }
    }
    StringJoiner dot("\n");
    dot.add("digraph {");
    dot.add("  node [shape=box];");
    for (const auto &[from, to] : edges) {
        dot.add("  {} -> {}", formatDotId(from), formatDotId(to));
    }
    dot.add("}");
    return dot.toString();
}
