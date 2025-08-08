// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Module.h"

#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

GlobalVar &Module::addGlobalVar(const_global_var_iterator pos, std::unique_ptr<GlobalVar> G) {
    return **globalVars_.insert(pos.base(), std::move(G));
}

std::unique_ptr<GlobalVar> Module::removeGlobalVar(const_global_var_iterator pos) {
    std::unique_ptr<GlobalVar> G = std::move(const_cast<std::unique_ptr<GlobalVar> &>(*pos.base()));
    globalVars_.erase(pos.base());
    return G;
}

void Module::clearGlobalVars() {
    globalVars_.clear();
}

Function &Module::addFunction(const_function_iterator pos, std::unique_ptr<Function> F) {
    return **functions_.insert(pos.base(), std::move(F));
}

std::unique_ptr<Function> Module::removeFunction(const_function_iterator pos) {
    std::unique_ptr<Function> F = std::move(const_cast<std::unique_ptr<Function> &>(*pos.base()));
    functions_.erase(pos.base());
    return F;
}

void Module::clearFunctions() {
    functions_.clear();
}

std::string Module::format() const {
    StringJoiner formatted("\n\n");
    for (const GlobalVar &G : globalVars(*this)) {
        formatted.add("{}", G);
    }
    for (const Function &F : functions(*this)) {
        formatted.add("{}", F);
    }
    return formatted.toString();
}
