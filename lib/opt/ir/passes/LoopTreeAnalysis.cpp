#include "mini-llvm/opt/ir/passes/LoopTreeAnalysis.h"

#include <algorithm>
#include <memory>
#include <queue>
#include <ranges>
#include <unordered_set>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm::ir;

namespace {

bool check(const Loop &loop, const DominatorTreeAnalysis &domTree) {
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
                    std::unordered_set<const BasicBlock *> S;
                    std::queue<const BasicBlock *> Q;
                    S.insert(&u);
                    Q.push(&u);
                    while (!Q.empty()) {
                        const BasicBlock *x = Q.front();
                        Q.pop();
                        for (const BasicBlock *y : predecessors(*x)) {
                            if (y != v && !S.contains(y)) {
                                S.insert(y);
                                Q.push(y);
                            }
                        }
                    }
                    S.insert(v);

                    Loop loop(std::move(S), v);

                    if (check(loop, domTree)) {
                        loops_.emplace_back(std::move(loop));
                    }
                }
            }
        }

        for (const Loop &loop : loops_) {
            nodes_(&loop) = LoopTreeNode(&loop, nullptr, {});
        }

        std::vector<LoopTreeNode *> nodes;
        for (LoopTreeNode &node : std::views::values(nodes_)) {
            nodes.push_back(&node);
        }
        std::ranges::sort(nodes, [](LoopTreeNode *lhs, LoopTreeNode *rhs) {
            return lhs->loop->blocks.size() > rhs->loop->blocks.size();
        });
        for (auto [i, x] : std::views::enumerate(nodes)) {
            LoopTreeNode *parent = nullptr;
            for (LoopTreeNode *y : nodes | std::views::take(i) | std::views::reverse) {
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

    const LoopTreeNode *node(const Loop &loop) const {
        return &nodes_[&loop];
    }

    const LoopTreeNode *rootNode() const {
        return &rootNode_;
    }

private:
    std::vector<Loop> loops_;
    HashMap<const Loop *, LoopTreeNode> nodes_;
    LoopTreeNode rootNode_{};
};

LoopTreeAnalysis::LoopTreeAnalysis() : impl_(std::make_unique<Impl>()) {}

LoopTreeAnalysis::~LoopTreeAnalysis() = default;

void LoopTreeAnalysis::runOnFunction(const Function &F) {
    impl_->runOnFunction(F);
}

const std::vector<Loop> &LoopTreeAnalysis::loops() const {
    return impl_->loops();
}

const LoopTreeNode *LoopTreeAnalysis::node(const Loop &loop) const {
    return impl_->node(loop);
}

const LoopTreeNode *LoopTreeAnalysis::rootNode() const {
    return impl_->rootNode();
}
