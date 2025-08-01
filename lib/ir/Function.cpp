#include "mini-llvm/ir/Function.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <optional>
#include <queue>
#include <ranges>
#include <stack>
#include <string>
#include <unordered_set>
#include <utility>

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
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

std::unordered_set<const BasicBlock *> findReachable(const Function &F) {
    std::unordered_set<const BasicBlock *> S;
    std::queue<const BasicBlock *> Q;
    S.insert(&F.entry());
    Q.push(&F.entry());
    while (!Q.empty()) {
        const BasicBlock *u = Q.front();
        Q.pop();
        for (const BasicBlock *v : successors(*u)) {
            if (S.insert(v).second) {
                Q.push(v);
            }
        }
    }
    return S;
}

void dfs(
    const BasicBlock *u,
    const HashMap<const BasicBlock *, std::unordered_set<const BasicBlock *>> &g,
    HashMap<const BasicBlock *, int> &dfn,
    HashMap<const BasicBlock *, int> &low,
    HashMap<const BasicBlock *, int> &scc,
    int &timer,
    int &sccCount,
    std::stack<const BasicBlock *> &S
) {
    dfn[u] = low[u] = timer;
    ++timer;
    S.push(u);
    for (const BasicBlock *v : g[u]) {
        if (dfn[v] == -1) {
            dfs(v, g, dfn, low, scc, timer, sccCount, S);
            low[u] = std::min(low[u], low[v]);
        } else if (scc[v] == -1) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (low[u] == dfn[u]) {
        const BasicBlock *v;
        do {
            v = S.top();
            S.pop();
            scc[v] = sccCount;
        } while (v != u);
        ++sccCount;
    }
}

std::unordered_set<const BasicBlock *> findNotInCycle(const Function &F) {
    HashMap<const BasicBlock *, std::unordered_set<const BasicBlock *>> g;
    for (const BasicBlock &B : F) {
        g.put(&B, {});
        for (const BasicBlock *succ : successors(B)) {
            g[&B].insert(succ);
        }
    }
    HashMap<const BasicBlock *, int> dfn, low;
    HashMap<const BasicBlock *, int> scc;
    int timer;
    int sccCount;
    std::stack<const BasicBlock *> S;
    for (const BasicBlock *v : std::views::keys(g)) {
        dfn.put(v, -1);
        low.put(v, -1);
        scc.put(v, -1);
    }
    timer = 0;
    sccCount = 0;
    for (const BasicBlock *u : std::views::keys(g)) {
        if (dfn[u] == -1) {
            dfs(u, g, dfn, low, scc, timer, sccCount, S);
        }
    }
    HashMap<int, int> sizes;
    for (int C = 0; C < sccCount; ++C) {
        sizes.put(C, 0);
    }
    for (const BasicBlock *v : std::views::keys(g)) {
        ++sizes[scc[v]];
    }
    std::unordered_set<const BasicBlock *> notInCycle;
    for (const BasicBlock *u : std::views::keys(g)) {
        if (sizes[scc[u]] == 1) {
            bool hasSelfLoop = false;
            for (const BasicBlock *v : g[u]) {
                if (v == u) {
                    hasSelfLoop = true;
                    break;
                }
            }
            if (!hasSelfLoop) {
                notInCycle.insert(u);
            }
        }
    }
    return notInCycle;
}

} // namespace

Function::Function(std::unique_ptr<FunctionType> functionType, Linkage linkage)
        : functionType_(std::move(functionType)), linkage_(linkage) {
    for (Type &paramType : paramTypes(*functionType_)) {
        args_.push_back(std::make_shared<Argument>(paramType.clone()));
    }
}

bool Function::hasAttr(Attribute attr) const {
    return std::ranges::contains(attrs_, attr);
}

void Function::setAttr(Attribute attr) {
    if (!std::ranges::contains(attrs_, attr)) {
        attrs_.push_back(attr);
    }
}

void Function::clearAttr(Attribute attr) {
    std::erase(attrs_, attr);
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
                if (incomingBlocks(*phi) != predecessors(B)) {
                    return false;
                }
            }
        }
    }
    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(*this);
    std::unordered_set<const BasicBlock *> reachable = findReachable(*this);
    for (const BasicBlock *B : reachable) {
        for (const Instruction &I : *B) {
            for (const UseBase &use : uses(I)) {
                if (auto *II = dynamic_cast<const Instruction *>(use.user())) {
                    if (!dynamic_cast<const Phi *>(II) && reachable.contains(II->parent()) && !domTree.dominates(I, *II)) {
                        return false;
                    }
                }
            }
        }
    }
    std::unordered_set<const BasicBlock *> notInCycle = findNotInCycle(*this);
    for (const BasicBlock &B : *this) {
        if (!notInCycle.contains(&B)) {
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
    for (Attribute attr : attrs(*this)) {
        formatted.add(specifier(attr));
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
