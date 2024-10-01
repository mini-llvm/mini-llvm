#pragma once

#include <format>
#include <iterator>
#include <list>
#include <memory>
#include <string>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::ir {

class Module {
public:
    class GlobalVars {
        using GlobalVarList = std::list<std::shared_ptr<GlobalVar>>;

    public:
        using iterator = IndirectIterator<GlobalVarList::iterator, GlobalVar>;
        using const_iterator = IndirectIterator<GlobalVarList::const_iterator, const GlobalVar>;
        using reverse_iterator = IndirectIterator<GlobalVarList::reverse_iterator, GlobalVar>;
        using const_reverse_iterator = IndirectIterator<GlobalVarList::const_reverse_iterator, const GlobalVar>;

        iterator begin() {
            return iterator(globalVars_.begin());
        }

        const_iterator begin() const {
            return const_iterator(globalVars_.begin());
        }

        iterator end() {
            return iterator(globalVars_.end());
        }

        const_iterator end() const {
            return const_iterator(globalVars_.end());
        }

        reverse_iterator rbegin() {
            return reverse_iterator(globalVars_.rbegin());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(globalVars_.rbegin());
        }

        reverse_iterator rend() {
            return reverse_iterator(globalVars_.rend());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(globalVars_.rend());
        }

        GlobalVar &front() {
            return *begin();
        }

        const GlobalVar &front() const {
            return *begin();
        }

        GlobalVar &back() {
            return *std::prev(end());
        }

        const GlobalVar &back() const {
            return *std::prev(end());
        }

        bool empty() const {
            return globalVars_.empty();
        }

        size_t size() const {
            return globalVars_.size();
        }

        GlobalVar &add(const_iterator pos, std::shared_ptr<GlobalVar> G);

        GlobalVar &prepend(std::shared_ptr<GlobalVar> G) {
            return add(begin(), std::move(G));
        }

        GlobalVar &append(std::shared_ptr<GlobalVar> G) {
            return add(end(), std::move(G));
        }

        void remove(const_iterator pos);

        void removeFirst() {
            remove(begin());
        }

        void removeLast() {
            remove(std::prev(end()));
        }

        void clear();

    private:
        GlobalVarList globalVars_;
    };

    class Functions {
        using FunctionList = std::list<std::shared_ptr<Function>>;

    public:
        using iterator = IndirectIterator<FunctionList::iterator, Function>;
        using const_iterator = IndirectIterator<FunctionList::const_iterator, const Function>;
        using reverse_iterator = IndirectIterator<FunctionList::reverse_iterator, Function>;
        using const_reverse_iterator = IndirectIterator<FunctionList::const_reverse_iterator, const Function>;

        iterator begin() {
            return iterator(functions_.begin());
        }

        const_iterator begin() const {
            return const_iterator(functions_.begin());
        }

        iterator end() {
            return iterator(functions_.end());
        }

        const_iterator end() const {
            return const_iterator(functions_.end());
        }

        reverse_iterator rbegin() {
            return reverse_iterator(functions_.rbegin());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(functions_.rbegin());
        }

        reverse_iterator rend() {
            return reverse_iterator(functions_.rend());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(functions_.rend());
        }

        Function &front() {
            return *begin();
        }

        const Function &front() const {
            return *begin();
        }

        Function &back() {
            return *std::prev(end());
        }

        const Function &back() const {
            return *std::prev(end());
        }

        bool empty() const {
            return functions_.empty();
        }

        size_t size() const {
            return functions_.size();
        }

        Function &add(const_iterator pos, std::shared_ptr<Function> F);

        Function &prepend(std::shared_ptr<Function> F) {
            return add(begin(), std::move(F));
        }

        Function &append(std::shared_ptr<Function> F) {
            return add(end(), std::move(F));
        }

        void remove(const_iterator pos);

        void removeFirst() {
            remove(begin());
        }

        void removeLast() {
            remove(std::prev(end()));
        }

        void clear();

    private:
        FunctionList functions_;
    };

    GlobalVars globalVars;
    Functions functions;

    std::string format() const;
};

} // namespace mini_llvm::ir

template <>
struct std::formatter<mini_llvm::ir::Module> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const mini_llvm::ir::Module &M, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", M.format());
    }
};
