// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/GlobalCodeMotion.h"

#include <cassert>
#include <iterator>
#include <memory>
#include <ranges>
#include <utility>

#include "mini-llvm/ir/Attribute/ReadNone.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/IndirectCall.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Phi.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/opt/ir/passes/LoopTreeAnalysis.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

// Cliff Click. 1995. Global code motion/global value numbering.
// In Proceedings of the ACM SIGPLAN 1995 conference on
// Programming language design and implementation (PLDI '95).
// Association for Computing Machinery, New York, NY, USA, 246-257.
// https://doi.org/10.1145/207110.207154

namespace {

void findDepths(const DTNode *node, int depth, HashMap<const BasicBlock *, int> &depths) {
    depths.put(node->block, depth);
    for (const DTNode *child : node->children) {
        findDepths(child, depth + 1, depths);
    }
}

void findDepths(const LTNode *node, int depth, HashMap<const BasicBlock *, int> &depths) {
    for (const BasicBlock *B : node->loop->blocks) {
        depths.put(B, depth);
    }
    for (const LTNode *child : node->children) {
        findDepths(child, depth + 1, depths);
    }
}

const BasicBlock *findLCA(
    const BasicBlock *u,
    const BasicBlock *v,
    const HashMap<const BasicBlock *, int> &depths,
    const HashMap<const BasicBlock *, const BasicBlock *> &parents
) {
    if (u == nullptr) {
        return v;
    }
    while (depths[u] > depths[v]) {
        u = parents[u];
    }
    while (depths[v] > depths[u]) {
        v = parents[v];
    }
    while (u != v) {
        u = parents[u];
        v = parents[v];
    }
    return u;
}

bool isCriticalCall(const Instruction &I) {
    if (auto *call = dynamic_cast<const Call *>(&I)) {
        const Function &callee = *call->callee();
        if (!callee.attr<ReadNone>()) {
            return true;
        }
    }
    return false;
}

bool isPinned(const Instruction &I) {
    return dynamic_cast<const Phi *>(&I)
        || dynamic_cast<const Terminator *>(&I)
        || dynamic_cast<const Alloca *>(&I)
        || dynamic_cast<const Load *>(&I)
        || dynamic_cast<const Store *>(&I)
        || dynamic_cast<const IndirectCall *>(&I)
        || isCriticalCall(I);
}

void scheduleEarly(
    const DTNode *node,
    const HashMap<const BasicBlock *, int> &domTreeDepths,
    HashMap<const Instruction *, const BasicBlock *> &blocks
) {
    for (const Instruction &I : *node->block) {
        if (!isPinned(I)) {
            blocks.put(&I, &I.parent()->parent()->entry());
            for (const UseBase *op : I.operands()) {
                if (auto *II = dynamic_cast<const Instruction *>(&**op)) {
                    if (domTreeDepths[blocks[II]] > domTreeDepths[blocks[&I]]) {
                        blocks[&I] = blocks[II];
                    }
                }
            }
        }
    }
    for (const DTNode *child : node->children) {
        scheduleEarly(child, domTreeDepths, blocks);
    }
}

void scheduleLate(
    const DTNode *node,
    const HashMap<const BasicBlock *, int> &domTreeDepths,
    const HashMap<const BasicBlock *, const BasicBlock *> &domTreeParents,
    const HashMap<const BasicBlock *, int> &loopTreeDepths,
    HashMap<const Instruction *, const BasicBlock *> &blocks
) {
    for (const DTNode *child : node->children) {
        scheduleLate(child, domTreeDepths, domTreeParents, loopTreeDepths, blocks);
    }
    for (const Instruction &I : std::views::reverse(*node->block)) {
        if (!isPinned(I)) {
            const BasicBlock *lca = nullptr;
            for (const UseBase &use : uses(I)) {
                if (auto *II = dynamic_cast<const Instruction *>(use.user())) {
                    if (auto *phi = dynamic_cast<const Phi *>(II)) {
                        for (Phi::ConstIncoming incoming : incomings(*phi)) {
                            if (&*incoming.value == &I) {
                                lca = findLCA(lca, &*incoming.block, domTreeDepths, domTreeParents);
                            }
                        }
                    } else {
                        lca = findLCA(lca, blocks[II], domTreeDepths, domTreeParents);
                    }
                }
            }
            if (lca == nullptr) {
                lca = I.parent();
            }
            const BasicBlock *best = lca,
                             *current = lca;
            for (;;) {
                if (loopTreeDepths[current] < loopTreeDepths[best]) {
                    best = current;
                }
                if (current == blocks[&I]) {
                    break;
                }
                current = domTreeParents[current];
            }
            blocks[&I] = best;
        }
    }
}

BasicBlock::const_iterator findInsertionPos(const Instruction &I, const BasicBlock &B) {
    auto i = B.begin();
    while (dynamic_cast<const Phi *>(&*i)) {
        ++i;
    }
    for (auto j = i; j != B.end(); ++j) {
        for (const UseBase *op : I.operands()) {
            if (&*j == &**op) {
                i = std::next(j);
                break;
            }
        }
    }
    return i;
}

} // namespace

bool GlobalCodeMotion::runOnFunction(Function &F) {
    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    HashMap<const BasicBlock *, int> domTreeDepths;
    findDepths(domTree.node(F.entry()), 0, domTreeDepths);

    HashMap<const BasicBlock *, const BasicBlock *> domTreeParents;
    for (const BasicBlock &B : F) {
        if (&B != &F.entry()) {
            domTreeParents.put(&B, domTree.node(B)->parent->block);
        }
    }

    LoopTreeAnalysis loopTree;
    loopTree.runOnFunction(F);

    HashMap<const BasicBlock *, int> loopTreeDepths;
    for (const BasicBlock &B : F) {
        loopTreeDepths.put(&B, 0);
    }
    for (const LTNode *node : loopTree.rootNode()->children) {
        findDepths(node, 1, loopTreeDepths);
    }

    HashMap<const Instruction *, const BasicBlock *> blocks;
    for (const BasicBlock &B : F) {
        for (const Instruction &I : B) {
            blocks.put(&I, I.parent());
        }
    }

    scheduleEarly(domTree.node(F.entry()), domTreeDepths, blocks);
    scheduleLate(domTree.node(F.entry()), domTreeDepths, domTreeParents, loopTreeDepths, blocks);

    bool changed = false;

    for (BasicBlock &B : F) {
        for (auto i = B.begin(); i != B.end();) {
            Instruction *I = &*i++;
            BasicBlock *best = const_cast<BasicBlock *>(blocks[I]);
            if (best != I->parent()) {
                std::shared_ptr<Instruction> tmp = share(*I);
                removeFromParent(*I);
                best->add(findInsertionPos(*I, *best), std::move(tmp));
                changed = true;
            }
        }
    }

    assert(F.isWellFormed());
    return changed;
}
