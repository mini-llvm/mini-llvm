// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/LoopTreeAnalysis.h"

#include <algorithm>
#include <memory>
#include <queue>
#include <ranges>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm::ir;

namespace {

bool isNaturalLoop(const Loop &loop, const DominatorTreeAnalysis &domTree) {
    for (const BasicBlock *B : loop.blocks) {
        if (!domTree.dominates(*loop.header, *B)) {
            return false;
        }
    }
    return true;
}

} // namespace

class LoopTreeAnalysis::Impl {
public:
    void runOnFunction(const Function &F) {
        DominatorTreeAnalysis domTree;
        domTree.runOnFunction(F);

        for (const BasicBlock &u : F) {
            for (const BasicBlock *v : successors(u)) {
                if (domTree.dominates(*v, u)) {
                    if (v == &u) {
                        Loop loop({&u}, &u);

                        loops_.emplace_back(std::move(loop));
                    } else {
                        std::unordered_set<const BasicBlock *> visited;
                        std::queue<const BasicBlock *> Q;
                        visited.insert(&u);
                        Q.push(&u);
                        while (!Q.empty()) {
                            const BasicBlock *x = Q.front();
                            Q.pop();
                            for (const BasicBlock *y : predecessors(*x)) {
                                if (y != v) {
                                    if (visited.insert(y).second) {
                                        Q.push(y);
                                    }
                                }
                            }
                        }
                        visited.insert(v);

                        Loop loop(std::move(visited), v);

                        if (isNaturalLoop(loop, domTree)) {
                            loops_.emplace_back(std::move(loop));
                        }
                    }
                }
            }
        }

        for (const Loop &loop : loops_) {
            nodes_.put(&loop, LTNode(&loop, nullptr, {}));
        }

        std::vector<LTNode *> nodes;
        for (LTNode &node : std::views::values(nodes_)) {
            nodes.push_back(&node);
        }
        std::ranges::sort(nodes, [](LTNode *lhs, LTNode *rhs) {
            return lhs->loop->blocks.size() > rhs->loop->blocks.size();
        });
        for (auto [i, x] : std::views::enumerate(nodes)) {
            LTNode *parent = nullptr;
            for (LTNode *y : nodes | std::views::take(i) | std::views::reverse) {
                if (y->loop->blocks.contains(*x->loop->blocks.begin())) {
                    parent = y;
                    break;
                }
            }
            if (parent) {
                x->parent = parent;
                parent->children.push_back(x);
            } else {
                rootNode_.children.push_back(x);
            }
        }
    }

    const std::vector<Loop> &loops() const {
        return loops_;
    }

    const LTNode *node(const Loop &loop) const {
        return &nodes_[&loop];
    }

    const LTNode *rootNode() const {
        return &rootNode_;
    }

private:
    std::vector<Loop> loops_;
    HashMap<const Loop *, LTNode> nodes_;
    LTNode rootNode_{};
};

LoopTreeAnalysis::LoopTreeAnalysis() : impl_(std::make_unique<Impl>()) {}

LoopTreeAnalysis::~LoopTreeAnalysis() = default;

void LoopTreeAnalysis::runOnFunction(const Function &F) {
    impl_->runOnFunction(F);
}

const std::vector<Loop> &LoopTreeAnalysis::loops() const {
    return impl_->loops();
}

const LTNode *LoopTreeAnalysis::node(const Loop &loop) const {
    return impl_->node(loop);
}

const LTNode *LoopTreeAnalysis::rootNode() const {
    return impl_->rootNode();
}
