// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Module.h"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm;
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

GlobalVar *mir::getGlobalVarByName(Module &M, std::string_view name) {
    return const_cast<GlobalVar *>(getGlobalVarByName(std::as_const(M), name));
}

const GlobalVar *mir::getGlobalVarByName(const Module &M, std::string_view name) {
    auto i = std::find_if(M.global_var_begin(), M.global_var_end(), [name](const GlobalVar &G) {
        return G.name() == name;
    });
    if (i != M.global_var_end()) {
        return &*i;
    }
    return nullptr;
}

Function *mir::getFunctionByName(Module &M, std::string_view name) {
    return const_cast<Function *>(getFunctionByName(std::as_const(M), name));
}

const Function *mir::getFunctionByName(const Module &M, std::string_view name) {
    auto i = std::find_if(M.function_begin(), M.function_end(), [name](const Function &F) {
        return F.name() == name;
    });
    if (i != M.function_end()) {
        return &*i;
    }
    return nullptr;
}
