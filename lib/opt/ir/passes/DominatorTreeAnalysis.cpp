#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"

#include <cstddef>
#include <memory>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"

using namespace mini_llvm::ir;

// Thomas Lengauer and Robert Endre Tarjan. 1979. A fast algorithm for finding
// dominators in a flowgraph. ACM Trans. Program. Lang. Syst. 1, 1 (July 1979),
// 121-141. https://doi.org/10.1145/357062.357071

namespace {

class LengauerTarjan {
public:
    LengauerTarjan(const Function &F,
                   std::unordered_map<const BasicBlock *, const BasicBlock *> &idom)
        : F_(F), idom_(idom) {}

    void operator()() {
        for (const BasicBlock &v : F_) {
            pred_[&v] = {};
            bucket_[&v] = {};
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
                idom_[v] = semi_[u] < semi_[v] ? u : parent_[w];
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
    std::unordered_map<const BasicBlock *, const BasicBlock *> &idom_;
    std::unordered_map<const BasicBlock *, const BasicBlock *> parent_, ancestor_, label_;
    std::unordered_map<const BasicBlock *, size_t> semi_;
    std::unordered_map<size_t, const BasicBlock *> vertex_;
    std::unordered_map<const BasicBlock *, std::vector<const BasicBlock *>> pred_, bucket_;
    size_t timer_{};

    void dfs(const BasicBlock *v) {
        semi_[v] = ++timer_;
        vertex_[semi_[v]] = v;
        label_[v] = v;
        for (const BasicBlock *w : successors(*v)) {
            if (!semi_.contains(w)) {
                parent_[w] = v;
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
        ancestor_[v] = u;
    }
};

void dfs(const DominatorTreeNode *u,
         std::unordered_map<const DominatorTreeNode *, size_t> &discover,
         std::unordered_map<const DominatorTreeNode *, size_t> &finish,
         size_t &timer) {
    discover[u] = ++timer;
    for (const DominatorTreeNode *v : u->children) {
        if (!discover.contains(v)) {
            dfs(v, discover, finish, timer);
        }
    }
    finish[u] = ++timer;
}

} // namespace

class DominatorTreeAnalysis::Impl {
public:
    void runOnFunction(const Function &F) {
        std::unordered_map<const BasicBlock *, const BasicBlock *> idom;
        LengauerTarjan(F, idom)();

        std::unordered_set<const BasicBlock *> S;
        std::queue<const BasicBlock *> Q;
        S.insert(&F.entry());
        Q.push(&F.entry());
        while (!Q.empty()) {
            const BasicBlock *u = Q.front();
            Q.pop();
            for (const BasicBlock *v : successors(*u)) {
                if (!S.contains(v)) {
                    S.insert(v);
                    Q.push(v);
                }
            }
        }

        for (const BasicBlock *v : S) {
            node_[v] = {v, nullptr, {}};
        }
        for (const BasicBlock *v : S) {
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
                idx_[&I] = idx;
            }
        }
    }

    const DominatorTreeNode *node(const BasicBlock &v) const {
        return &node_.at(&v);
    }

    bool dominates(const BasicBlock &u, const BasicBlock &v) const {
        return discover_.at(node(u)) <= discover_.at(node(v)) && finish_.at(node(u)) >= finish_.at(node(v));
    }

    bool dominates(const Instruction &u, const Instruction &v) const {
        if (u.parent() != v.parent()) {
            return dominates(*u.parent(), *v.parent());
        } else {
            return idx_.at(&u) <= idx_.at(&v);
        }
    }

private:
    std::unordered_map<const BasicBlock *, DominatorTreeNode> node_;
    std::unordered_map<const DominatorTreeNode *, size_t> discover_, finish_;
    std::unordered_map<const Instruction *, size_t> idx_;
};

DominatorTreeAnalysis::DominatorTreeAnalysis() : impl_(std::make_unique<Impl>()) {}

DominatorTreeAnalysis::~DominatorTreeAnalysis() = default;

void DominatorTreeAnalysis::runOnFunction(const Function &F) {
    impl_->runOnFunction(F);
}

const DominatorTreeNode *DominatorTreeAnalysis::node(const BasicBlock &v) const {
    return impl_->node(v);
}

bool DominatorTreeAnalysis::dominates(const BasicBlock &u, const BasicBlock &v) const {
    return impl_->dominates(u, v);
}

bool DominatorTreeAnalysis::dominates(const Instruction &u, const Instruction &v) const {
    return impl_->dominates(u, v);
}
