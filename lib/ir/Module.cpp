// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Module.h"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

GlobalVar &Module::addGlobalVar(const_global_var_iterator pos, std::shared_ptr<GlobalVar> G) {
    return **globalVars_.insert(pos.base(), std::move(G));
}

void Module::removeGlobalVar(const_global_var_iterator pos) {
    globalVars_.erase(pos.base());
}

void Module::clearGlobalVars() {
    globalVars_.clear();
}

Function &Module::addFunction(const_function_iterator pos, std::shared_ptr<Function> F) {
    return **functions_.insert(pos.base(), std::move(F));
}

void Module::removeFunction(const_function_iterator pos) {
    functions_.erase(pos.base());
}

void Module::clearFunctions() {
    functions_.clear();
}

bool Module::isWellFormed() const {
    for (const GlobalVar &G : globalVars(*this)) {
        if (!G.isWellFormed()) {
            return false;
        }
    }
    for (const Function &F : functions(*this)) {
        if (!F.isWellFormed()) {
            return false;
        }
    }
    return true;
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

GlobalVar *ir::getGlobalVarByName(Module &M, std::string_view name) {
    return const_cast<GlobalVar *>(getGlobalVarByName(std::as_const(M), name));
}

const GlobalVar *ir::getGlobalVarByName(const Module &M, std::string_view name) {
    auto i = std::find_if(M.global_var_begin(), M.global_var_end(), [name](const GlobalVar &G) {
        return G.name() == name;
    });
    if (i != M.global_var_end()) {
        return &*i;
    }
    return nullptr;
}

Function *ir::getFunctionByName(Module &M, std::string_view name) {
    return const_cast<Function *>(getFunctionByName(std::as_const(M), name));
}

const Function *ir::getFunctionByName(const Module &M, std::string_view name) {
    auto i = std::find_if(M.function_begin(), M.function_end(), [name](const Function &F) {
        return F.name() == name;
    });
    if (i != M.function_end()) {
        return &*i;
    }
    return nullptr;
}
