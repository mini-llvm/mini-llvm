// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/AttributeDeduction.h"

#include <algorithm>
#include <cassert>
#include <memory>
#include <queue>
#include <ranges>
#include <stack>
#include <unordered_set>
#include <vector>

#include "mini-llvm/ir/Attribute/ReadNone.h"
#include "mini-llvm/ir/Attribute/ReadOnly.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Alloca.h"
#include "mini-llvm/ir/Instruction/Call.h"
#include "mini-llvm/ir/Instruction/IndirectCall.h"
#include "mini-llvm/ir/Instruction/Load.h"
#include "mini-llvm/ir/Instruction/Store.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

void dfs(
    const Function *u,
    const HashMap<const Function *, std::unordered_set<const Function *>> &g,
    HashMap<const Function *, int> &dfn,
    HashMap<const Function *, int> &low,
    HashMap<const Function *, int> &scc,
    int &timer,
    int &sccCount,
    std::stack<const Function *> &S
) {
    dfn[u] = low[u] = timer;
    ++timer;
    S.push(u);
    for (const Function *v : g[u]) {
        if (dfn[v] == -1) {
            dfs(v, g, dfn, low, scc, timer, sccCount, S);
            low[u] = std::min(low[u], low[v]);
        } else if (scc[v] == -1) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (low[u] == dfn[u]) {
        const Function *v;
        do {
            v = S.top();
            S.pop();
            scc[v] = sccCount;
        } while (v != u);
        ++sccCount;
    }
}

} // namespace

bool AttributeDeduction::runOnModule(Module &M) {
    HashMap<const Function *, std::unordered_set<const Function *>> callGraph;
    for (const Function &F : functions(M)) {
        callGraph.put(&F, {});
    }
    for (const Function &F : functions(M)) {
        for (const BasicBlock &B : F) {
            for (const Instruction &I : B) {
                if (auto *call = dynamic_cast<const Call *>(&I)) {
                    callGraph[&F].insert(&*call->callee());
                }
            }
        }
    }

    HashMap<const Function *, int> dfn, low;
    HashMap<const Function *, int> scc;
    for (auto v : std::views::keys(callGraph)) {
        dfn.put(v, -1);
        low.put(v, -1);
        scc.put(v, -1);
    }
    int timer = 0;
    int sccCount = 0;
    std::stack<const Function *> S;
    for (auto u : std::views::keys(callGraph)) {
        if (dfn[u] == -1) {
            dfs(u, callGraph, dfn, low, scc, timer, sccCount, S);
        }
    }

    std::unordered_set<int> notReadNone, notReadOnly;
    for (const Function &F : functions(M)) {
        if (!F.attr<ReadNone>()) {
            if (!F.isDeclaration()) {
                bool readNone = true;
                for (const BasicBlock &B : F) {
                    for (const Instruction &I : B) {
                        if (auto *load = dynamic_cast<const Load *>(&I); load && !dynamic_cast<const Alloca *>(&*load->ptr())) {
                            readNone = false;
                            break;
                        }
                        if (auto *store = dynamic_cast<const Store *>(&I); store && !dynamic_cast<const Alloca *>(&*store->ptr())) {
                            readNone = false;
                            break;
                        }
                        if (dynamic_cast<const IndirectCall *>(&I)) {
                            readNone = false;
                            break;
                        }
                    }
                }
                if (!readNone) {
                    notReadNone.insert(scc[&F]);
                }
            } else {
                notReadNone.insert(scc[&F]);
            }
        }
        if (!F.attr<ReadOnly>()) {
            if (!F.isDeclaration()) {
                bool readOnly = true;
                for (const BasicBlock &B : F) {
                    for (const Instruction &I : B) {
                        if (auto *store = dynamic_cast<const Store *>(&I); store && !dynamic_cast<const Alloca *>(&*store->ptr())) {
                            readOnly = false;
                            break;
                        }
                        if (dynamic_cast<const IndirectCall *>(&I)) {
                            readOnly = false;
                            break;
                        }
                    }
                }
                if (!readOnly) {
                    notReadOnly.insert(scc[&F]);
                }
            } else {
                notReadOnly.insert(scc[&F]);
            }
        }
    }

    HashMap<int, std::unordered_set<int>> sccGraph;
    for (int C = 0; C < sccCount; ++C) {
        sccGraph.put(C, {});
    }
    for (const auto &[u, N] : callGraph) {
        for (auto v : N) {
            if (scc[u] != scc[v]) {
                sccGraph[scc[u]].insert(scc[v]);
            }
        }
    }

    HashMap<int, int> in;
    for (auto v : std::views::keys(sccGraph)) {
        in.put(v, 0);
    }
    for (const auto &[u, N] : sccGraph) {
        for (auto v : N) {
            ++in[v];
        }
    }
    std::queue<int> Q;
    for (auto v : std::views::keys(sccGraph)) {
        if (in[v] == 0) {
            Q.push(v);
        }
    }
    std::vector<int> topo;
    while (!Q.empty()) {
        auto u = Q.front();
        Q.pop();
        topo.push_back(u);
        for (auto v : sccGraph[u]) {
            --in[v];
            if (in[v] == 0) {
                Q.push(v);
            }
        }
    }

    for (auto u : std::views::reverse(topo)) {
        if (!notReadNone.contains(u)) {
            bool readNone = true;
            for (auto v : sccGraph[u]) {
                if (notReadNone.contains(v)) {
                    readNone = false;
                    break;
                }
            }
            if (!readNone) {
                notReadNone.insert(u);
            }
        }
        if (!notReadOnly.contains(u)) {
            bool readOnly = true;
            for (auto v : sccGraph[u]) {
                if (notReadOnly.contains(v)) {
                    readOnly = false;
                    break;
                }
            }
            if (!readOnly) {
                notReadOnly.insert(u);
            }
        }
    }

    bool changed = false;

    for (Function &F : functions(M)) {
        if (!F.attr<ReadNone>() && !notReadNone.contains(scc[&F])) {
            F.removeAttr<ReadOnly>();
            F.addAttr(std::make_unique<ReadNone>());
            changed = true;
        }
        if (!F.attr<ReadNone>() && !F.attr<ReadOnly>() && !notReadOnly.contains(scc[&F])) {
            F.addAttr(std::make_unique<ReadOnly>());
            changed = true;
        }
    }

    assert(M.isWellFormed());
    return changed;
}
