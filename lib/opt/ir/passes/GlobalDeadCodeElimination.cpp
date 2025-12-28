// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/GlobalDeadCodeElimination.h"

#include <cassert>
#include <queue>
#include <unordered_set>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/ArrayConstant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Use.h"

using namespace mini_llvm::ir;

namespace {

void collect(const Constant &C, std::unordered_set<const GlobalValue *> &visited, std::queue<const GlobalValue *> &Q) {
    if (!dynamic_cast<const ArrayConstant *>(&C)) {
        if (auto *value = dynamic_cast<const GlobalValue *>(&C)) {
            if (visited.insert(value).second) {
                Q.push(value);
            }
        }
        return;
    }
    for (const Use<Constant> &element : elements(*static_cast<const ArrayConstant *>(&C))) {
        collect(*element, visited, Q);
    }
}

} // namespace

bool GlobalDeadCodeElimination::runOnModule(Module &M) {
    std::unordered_set<const GlobalValue *> visited;
    std::queue<const GlobalValue *> Q;
    for (const GlobalVar &G : globalVars(M)) {
        if (G.linkage() == Linkage::kExternal && !G.isDeclaration()) {
            visited.insert(&G);
            Q.push(&G);
        }
    }
    for (const Function &F : functions(M)) {
        if (F.linkage() == Linkage::kExternal && !F.isDeclaration()) {
            visited.insert(&F);
            Q.push(&F);
        }
    }
    while (!Q.empty()) {
        const GlobalValue *value = Q.front();
        Q.pop();
        if (auto *F = dynamic_cast<const Function *>(value)) {
            for (const BasicBlock &B : *F) {
                for (const Instruction &I : B) {
                    for (const UseBase *op : I.operands()) {
                        if (auto *value2 = dynamic_cast<const GlobalValue *>(&**op)) {
                            if (visited.insert(value2).second) {
                                Q.push(value2);
                            }
                        }
                    }
                }
            }
            continue;
        }
        if (auto *G = dynamic_cast<const GlobalVar *>(value)) {
            if (!G->isDeclaration()) {
                collect(G->initializer(), visited, Q);
            }
            continue;
        }
    }

    bool changed = false;

    for (auto i = M.global_var_begin(); i != M.global_var_end();) {
        if (!visited.contains(&*i)) {
            M.removeGlobalVar(i++);
            changed = true;
            continue;
        }
        ++i;
    }
    for (auto i = M.function_begin(); i != M.function_end();) {
        if (!visited.contains(&*i)) {
            M.removeFunction(i++);
            changed = true;
            continue;
        }
        ++i;
    }

    assert(M.isWellFormed());
    return changed;
}
