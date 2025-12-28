// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

#include <cstddef>
#include <memory>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/utils/Dot.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::string ir::toDot(const DTNode *root) {
    std::vector<std::pair<std::string, std::string>> edges;
    std::queue<const DTNode *> Q;
    Q.push(root);
    while (!Q.empty()) {
        const DTNode *node = Q.front();
        Q.pop();
        for (const DTNode *child : node->children) {
            edges.emplace_back(node->block->formatAsOperand(), child->block->formatAsOperand());
            Q.push(child);
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

// Thomas Lengauer and Robert Endre Tarjan. 1979.
// A fast algorithm for finding dominators in a flowgraph.
// ACM Trans. Program. Lang. Syst. 1, 1 (Jan. 1979), 121-141.
// https://doi.org/10.1145/357062.357071

namespace {

class LengauerTarjan {
public:
    LengauerTarjan(const Function &F,
                   HashMap<const BasicBlock *, const BasicBlock *> &idom)
        : F_(F), idom_(idom) {}

    void operator()() {
        for (const BasicBlock &v : F_) {
            pred_.put(&v, {});
            bucket_.put(&v, {});
        }
        dfs(&F_.entry());
        for (size_t i = timer_; i >= 2; --i) {
            const BasicBlock *w = vertex_[i];
            for (const BasicBlock *v : pred_[w]) {
                const BasicBlock *u = eval(v);
                if (semi_[u] < semi_[w]) {
                    semi_[w] = semi_[u];
                }
            }
            bucket_[vertex_[semi_[w]]].push_back(w);
            link(parent_[w], w);
            for (const BasicBlock *v : bucket_[parent_[w]]) {
                const BasicBlock *u = eval(v);
                idom_.put(v, semi_[u] < semi_[v] ? u : parent_[w]);
            }
        }
        for (size_t i = 2; i <= timer_; ++i) {
            const BasicBlock *w = vertex_[i];
            if (idom_[w] != vertex_[semi_[w]]) {
                idom_[w] = idom_[idom_[w]];
            }
        }
    }

private:
    const Function &F_;
    HashMap<const BasicBlock *, const BasicBlock *> &idom_;
    HashMap<const BasicBlock *, const BasicBlock *> parent_, ancestor_, label_;
    HashMap<const BasicBlock *, size_t> semi_;
    HashMap<size_t, const BasicBlock *> vertex_;
    HashMap<const BasicBlock *, std::vector<const BasicBlock *>> pred_, bucket_;
    size_t timer_{};

    void dfs(const BasicBlock *v) {
        semi_.put(v, ++timer_);
        vertex_.put(semi_[v], v);
        label_.put(v, v);
        for (const BasicBlock *w : successors(*v)) {
            if (!semi_.contains(w)) {
                parent_.put(w, v);
                dfs(w);
            }
            pred_[w].push_back(v);
        }
    }

    void compress(const BasicBlock *v) {
        if (ancestor_.contains(ancestor_[v])) {
            compress(ancestor_[v]);
            if (semi_[label_[ancestor_[v]]] < semi_[label_[v]]) {
                label_[v] = label_[ancestor_[v]];
            }
            ancestor_[v] = ancestor_[ancestor_[v]];
        }
    }

    const BasicBlock *eval(const BasicBlock *v) {
        if (!ancestor_.contains(v)) {
            return v;
        } else {
            compress(v);
            return label_[v];
        }
    }

    void link(const BasicBlock *u, const BasicBlock *v) {
        ancestor_.put(v, u);
    }
};

void dfs(const DTNode *u,
         HashMap<const DTNode *, size_t> &discover,
         HashMap<const DTNode *, size_t> &finish,
         size_t &timer) {
    discover.put(u, ++timer);
    for (const DTNode *v : u->children) {
        if (!discover.contains(v)) {
            dfs(v, discover, finish, timer);
        }
    }
    finish.put(u, ++timer);
}

} // namespace

class DominatorTreeAnalysis::Impl {
public:
    void runOnFunction(const Function &F) {
        HashMap<const BasicBlock *, const BasicBlock *> idom;
        LengauerTarjan(F, idom)();

        std::unordered_set<const BasicBlock *> visited;
        std::queue<const BasicBlock *> Q;
        visited.insert(&F.entry());
        Q.push(&F.entry());
        while (!Q.empty()) {
            const BasicBlock *u = Q.front();
            Q.pop();
            for (const BasicBlock *v : successors(*u)) {
                if (visited.insert(v).second) {
                    Q.push(v);
                }
            }
        }

        for (const BasicBlock *v : visited) {
            node_.put(v, {v, nullptr, {}});
        }
        for (const BasicBlock *v : visited) {
            if (v != &F.entry()) {
                const BasicBlock *u = idom[v];
                node_[v].parent = &node_[u];
                node_[u].children.push_back(&node_[v]);
            }
        }

        size_t timer = 0;
        dfs(&node_[&F.entry()], discover_, finish_, timer);

        for (const BasicBlock &B : F) {
            for (auto [idx, I] : std::views::enumerate(B)) {
                idx_.put(&I, idx);
            }
        }
    }

    const DTNode *node(const BasicBlock &v) const {
        return &node_[&v];
    }

    bool dominates(const BasicBlock &u, const BasicBlock &v) const {
        return discover_[node(u)] <= discover_[node(v)] && finish_[node(u)] >= finish_[node(v)];
    }

    bool dominates(const Instruction &u, const Instruction &v) const {
        if (u.parent() != v.parent()) {
            return dominates(*u.parent(), *v.parent());
        } else {
            return idx_[&u] <= idx_[&v];
        }
    }

private:
    HashMap<const BasicBlock *, DTNode> node_;
    HashMap<const DTNode *, size_t> discover_, finish_;
    HashMap<const Instruction *, size_t> idx_;
};

DominatorTreeAnalysis::DominatorTreeAnalysis() : impl_(std::make_unique<Impl>()) {}

DominatorTreeAnalysis::~DominatorTreeAnalysis() = default;

void DominatorTreeAnalysis::runOnFunction(const Function &F) {
    impl_->runOnFunction(F);
}

const DTNode *DominatorTreeAnalysis::node(const BasicBlock &v) const {
    return impl_->node(v);
}

bool DominatorTreeAnalysis::dominates(const BasicBlock &u, const BasicBlock &v) const {
    return impl_->dominates(u, v);
}

bool DominatorTreeAnalysis::dominates(const Instruction &u, const Instruction &v) const {
    return impl_->dominates(u, v);
}
