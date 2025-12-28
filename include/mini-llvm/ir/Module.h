// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <format>
#include <iterator>
#include <list>
#include <memory>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Module {
    using GlobalVarList = std::list<std::shared_ptr<GlobalVar>>;
    using FunctionList = std::list<std::shared_ptr<Function>>;

public:
    using global_var_iterator = IndirectIterator<GlobalVarList::iterator, GlobalVar>;
    using const_global_var_iterator = IndirectIterator<GlobalVarList::const_iterator, const GlobalVar>;

    using function_iterator = IndirectIterator<FunctionList::iterator, Function>;
    using const_function_iterator = IndirectIterator<FunctionList::const_iterator, const Function>;

    global_var_iterator global_var_begin() {
        return global_var_iterator(globalVars_.begin());
    }

    const_global_var_iterator global_var_begin() const {
        return const_global_var_iterator(globalVars_.begin());
    }

    global_var_iterator global_var_end() {
        return global_var_iterator(globalVars_.end());
    }

    const_global_var_iterator global_var_end() const {
        return const_global_var_iterator(globalVars_.end());
    }

    bool global_var_empty() const {
        return globalVars_.empty();
    }

    size_t global_var_size() const {
        return globalVars_.size();
    }

    GlobalVar &addGlobalVar(const_global_var_iterator pos, std::shared_ptr<GlobalVar> G);

    GlobalVar &prependGlobalVar(std::shared_ptr<GlobalVar> G) {
        return addGlobalVar(global_var_begin(), std::move(G));
    }

    GlobalVar &appendGlobalVar(std::shared_ptr<GlobalVar> G) {
        return addGlobalVar(global_var_end(), std::move(G));
    }

    void removeGlobalVar(const_global_var_iterator pos);

    void removeFirstGlobalVar() {
        removeGlobalVar(global_var_begin());
    }

    void removeLastGlobalVar() {
        removeGlobalVar(std::prev(global_var_end()));
    }

    void clearGlobalVars();

    function_iterator function_begin() {
        return function_iterator(functions_.begin());
    }

    const_function_iterator function_begin() const {
        return const_function_iterator(functions_.begin());
    }

    function_iterator function_end() {
        return function_iterator(functions_.end());
    }

    const_function_iterator function_end() const {
        return const_function_iterator(functions_.end());
    }

    bool function_empty() const {
        return functions_.empty();
    }

    size_t function_size() const {
        return functions_.size();
    }

    Function &addFunction(const_function_iterator pos, std::shared_ptr<Function> F);

    Function &prependFunction(std::shared_ptr<Function> F) {
        return addFunction(function_begin(), std::move(F));
    }

    Function &appendFunction(std::shared_ptr<Function> F) {
        return addFunction(function_end(), std::move(F));
    }

    void removeFunction(const_function_iterator pos);

    void removeFirstFunction() {
        removeFunction(function_begin());
    }

    void removeLastFunction() {
        removeFunction(std::prev(function_end()));
    }

    void clearFunctions();

    bool isWellFormed() const;

    std::string format() const;

private:
    GlobalVarList globalVars_;
    FunctionList functions_;
};

inline auto globalVars(Module &M) {
    return std::ranges::subrange(M.global_var_begin(), M.global_var_end());
}

inline auto globalVars(const Module &M) {
    return std::ranges::subrange(M.global_var_begin(), M.global_var_end());
}

inline auto functions(Module &M) {
    return std::ranges::subrange(M.function_begin(), M.function_end());
}

inline auto functions(const Module &M) {
    return std::ranges::subrange(M.function_begin(), M.function_end());
}

MINI_LLVM_EXPORT GlobalVar *getGlobalVarByName(Module &M, std::string_view name);
MINI_LLVM_EXPORT const GlobalVar *getGlobalVarByName(const Module &M, std::string_view name);
MINI_LLVM_EXPORT Function *getFunctionByName(Module &M, std::string_view name);
MINI_LLVM_EXPORT const Function *getFunctionByName(const Module &M, std::string_view name);

} // namespace mini_llvm::ir

template <>
struct std::formatter<mini_llvm::ir::Module> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::ir::Module &M, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", M.format());
    }
};
