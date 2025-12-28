// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/mir/passes/BasicBlockReordering.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <limits>
#include <memory>
#include <random>
#include <ranges>
#include <utility>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/Instruction/CmpBr.h"
#include "mini-llvm/mir/Instruction/CmpZBr.h"
#include "mini-llvm/mir/Instruction/CondBr.h"
#include "mini-llvm/opt/mir/passes/BranchPredictionAnalysis.h"
#include "mini-llvm/utils/HashMap.h"
#include "mini-llvm/utils/Matrix.h"
#include "mini-llvm/utils/PCG32.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

namespace {

// Held-Karp algorithm

constexpr bool test(size_t x, size_t i) {
    return (x >> i) & 1;
}

constexpr size_t clear(size_t x, size_t i) {
    return x & ~(static_cast<size_t>(1) << i);
}

constexpr size_t clear(size_t x, size_t i, size_t j) {
    return x & ~(static_cast<size_t>(1) << i) & ~(static_cast<size_t>(1) << j);
}

constexpr size_t full(size_t n) {
    return (static_cast<size_t>(1) << n) - 1;
}

std::vector<size_t> dp(size_t n, const Matrix<double> &D, size_t s) {
    assert(n > 2);
    Matrix<double> f(full(n) + 1, n);
    for (size_t v = 0; v < n; ++v) {
        if (v != s) {
            f[0, v] = D[s, v];
        }
    }
    for (size_t S = 1; S <= full(n); ++S) {
        for (size_t v = 0; v < n; ++v) {
            if (v != s && !test(S, s) && !test(S, v)) {
                f[S, v] = std::numeric_limits<double>::infinity();
                for (size_t u = 0; u < n; ++u) {
                    if (test(S, u)) {
                        f[S, v] = std::min(f[S, v], f[clear(S, u), u] + D[u, v]);
                    }
                }
            }
        }
    }
    size_t t{};
    double min = std::numeric_limits<double>::infinity();
    for (size_t v = 0; v < n; ++v) {
        if (v != s && f[clear(full(n), s, v), v] < min) {
            t = v;
            min = f[clear(full(n), s, v), v];
        }
    }
    size_t S = clear(full(n), s, t);
    std::vector<size_t> path;
    path.push_back(t);
    for (size_t i = 1; i < n - 1; ++i) {
        size_t v{};
        double min = std::numeric_limits<double>::infinity();
        for (size_t u = 0; u < n; ++u) {
            if (u != t && test(S, u) && f[clear(S, u), u] + D[u, t] < min) {
                v = u;
                min = f[clear(S, u), u] + D[u, t];
            }
        }
        t = v;
        S = clear(S, v);
        path.push_back(t);
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());
    return path;
}

// Ant Colony Optimization (ACO)

template <typename RNG>
std::vector<size_t> aco(
    size_t n,
    const Matrix<double> &D,
    size_t s,
    size_t m,
    double alpha,
    double beta,
    double rho,
    double Q,
    const std::vector<size_t> &initialPath,
    size_t maxIter,
    RNG &&rng
) {
    Matrix<double> tau(n, n, 1.);
    std::vector<size_t> bestPath = initialPath;
    double bestPathLength = 0.;
    for (size_t i = 1; i < n; ++i) {
        bestPathLength += D[initialPath[i - 1], initialPath[i]];
    }
    for (size_t iter = 0; iter < maxIter; ++iter) {
        std::vector<std::vector<size_t>> paths;
        for (size_t i = 0; i < m; ++i) {
            std::vector<size_t> path;
            path.push_back(s);
            for (size_t j = 1; j < n; ++j) {
                std::vector<double> weights;
                for (size_t k = 0; k < n; ++k) {
                    weights.push_back(pow(tau[path[j - 1], k], alpha) * pow(1. / (D[path[j - 1], k] + 1e-10), beta));
                }
                for (size_t k = 0; k < j; ++k) {
                    weights[path[k]] = 0.;
                }
                path.push_back(std::discrete_distribution<size_t>(weights.begin(), weights.end())(rng));
            }
            paths.push_back(path);
        }
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                tau[i, j] *= 1. - rho;
            }
        }
        for (size_t i = 0; i < m; ++i) {
            double pathLength = 0.;
            for (size_t j = 1; j < n; ++j) {
                pathLength += D[paths[i][j - 1], paths[i][j]];
            }
            for (size_t j = 1; j < n; ++j) {
                tau[paths[i][j - 1], paths[i][j]] += Q / (pathLength + 1e-10);
            }
            if (pathLength < bestPathLength) {
                bestPath = paths[i];
                bestPathLength = pathLength;
            }
        }
    }
    return bestPath;
}

void swapOperands(BasicBlockOperand &lhs, BasicBlockOperand &rhs) {
    BasicBlock *tmp = &*lhs;
    lhs.set(&*rhs);
    rhs.set(tmp);
}

} // namespace

bool BasicBlockReordering::runOnFunction(Function &F) {
    size_t n = F.size();
    if (n <= 2) {
        return false;
    }

    BranchPredictionAnalysis predictor;
    predictor.runOnFunction(F);

    HashMap<const BasicBlock *, size_t> indices;
    for (auto [i, B] : std::views::enumerate(F)) {
        indices.put(&B, i);
    }

    Matrix<double> D(n, n, 1e+10);
    for (const BasicBlock &B : F) {
        for (const BasicBlock *succ : successors(B)) {
            D[indices[&B], indices[succ]] = static_cast<double>(!predictor.predict(B, *succ));
        }
    }

    std::vector<size_t> initialPath;
    for (size_t i = 0; i < n; ++i) {
        initialPath.push_back(i);
    }

    std::vector<size_t> bestPath;

    if (n <= 16) {
        bestPath = dp(n, D, 0);
    } else {
        bestPath = aco(n, D, 0, 10, 1., 1., 0.1, 1., initialPath, 100, PCG32(42));
    }

    bool changed = false;

    if (bestPath != initialPath) {
        std::vector<std::unique_ptr<BasicBlock>> tmp;
        while (!F.empty()) {
            tmp.push_back(F.removeFirst());
        }
        for (size_t i : bestPath) {
            F.append(std::move(tmp[i]));
        }
        changed = true;
    }

    for (auto i = F.begin(), j = std::next(F.begin()); j != F.end(); ++i, ++j) {
        if (auto *br = dynamic_cast<CondBr *>(&i->back())) {
            if (&*j == &*br->falseDest()) {
                br->setCond(inverted(br->cond()));
                swapOperands(br->trueDest(), br->falseDest());
                changed = true;
                continue;
            }
            continue;
        }
        if (auto *br = dynamic_cast<CmpBr *>(&i->back())) {
            if (&*j == &*br->falseDest()) {
                br->setCond(inverted(br->cond()));
                swapOperands(br->trueDest(), br->falseDest());
                changed = true;
                continue;
            }
            continue;
        }
        if (auto *br = dynamic_cast<CmpZBr *>(&i->back())) {
            if (&*j == &*br->falseDest()) {
                br->setCond(inverted(br->cond()));
                swapOperands(br->trueDest(), br->falseDest());
                changed = true;
                continue;
            }
            continue;
        }
    }

    return changed;
}
