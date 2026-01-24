// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/BranchPredictionAnalysis.h"

#include <cstddef>
#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/utils/HashCombine.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

using Edge = std::pair<const BasicBlock *, const BasicBlock *>;

struct EdgeHash {
    constexpr size_t operator()(const Edge &e) const noexcept {
        size_t seed = 0;

        hash_combine(seed, e.first);
        hash_combine(seed, e.second);

        return seed;
    }
};

enum class Color {
    kWhite,
    kGray,
    kBlack,
};

void dfs(const BasicBlock *u,
         HashMap<const BasicBlock *, Color> &colors,
         std::unordered_set<Edge, EdgeHash> &backEdges) {
    colors[u] = Color::kGray;
    for (const BasicBlock *v : successors(*u)) {
        if (colors[v] == Color::kWhite) {
            dfs(v, colors, backEdges);
        } else if (colors[v] == Color::kGray) {
            backEdges.emplace(u, v);
        }
    }
    colors[u] = Color::kBlack;
}

} // namespace

class BranchPredictionAnalysis::Impl {
public:
    void runOnFunction(const Function &F) {
        HashMap<const BasicBlock *, Color> colors;
        std::unordered_set<Edge, EdgeHash> backEdges;

        for (const BasicBlock &B : F) {
            colors.put(&B, Color::kWhite);
        }

        for (const BasicBlock &B : F) {
            if (colors[&B] == Color::kWhite) {
                dfs(&B, colors, backEdges);
            }
        }

        for (const BasicBlock &B : F) {
            std::unordered_set<const BasicBlock *> predictions;

            for (const BasicBlock *succ : successors(B)) {
                if (backEdges.contains(Edge(&B, succ))) {
                    predictions.insert(succ);
                }
            }

            if (predictions.empty()) {
                for (const BasicBlock *succ : successors(B)) {
                    predictions.insert(succ);
                }
            }

            for (const BasicBlock *prediction : predictions) {
                predictions_.emplace(&B, prediction);
            }
        }
    }

    bool predict(const BasicBlock &B, const BasicBlock &succ) const {
        return predictions_.contains(Edge(&B, &succ));
    }

private:
    std::unordered_set<Edge, EdgeHash> predictions_;
};

BranchPredictionAnalysis::BranchPredictionAnalysis() : impl_(std::make_unique<Impl>()) {}

BranchPredictionAnalysis::~BranchPredictionAnalysis() = default;

void BranchPredictionAnalysis::runOnFunction(const Function &F) {
    impl_->runOnFunction(F);
}

bool BranchPredictionAnalysis::predict(const BasicBlock &B, const BasicBlock &succ) const {
    return impl_->predict(B, succ);
}
