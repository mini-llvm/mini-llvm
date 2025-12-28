// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <cstddef>
#include <format>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/mc/Statement.h"
#include "mini-llvm/mc/Symbol.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT GlobalValue {
    using StatementList = std::list<std::unique_ptr<Statement>>;

public:
    using iterator = IndirectIterator<StatementList::iterator, Statement>;
    using const_iterator = IndirectIterator<StatementList::const_iterator, const Statement>;
    using reverse_iterator = IndirectIterator<StatementList::reverse_iterator, Statement>;
    using const_reverse_iterator = IndirectIterator<StatementList::const_reverse_iterator, const Statement>;

    GlobalValue(Symbol symbol, std::string section, bool isGlobal, int alignment)
        : symbol_(std::move(symbol)), section_(std::move(section)), isGlobal_(isGlobal), alignment_(alignment) {}

    GlobalValue(Symbol symbol, std::string section, bool isGlobal)
        : symbol_(std::move(symbol)), section_(std::move(section)), isGlobal_(isGlobal), alignment_(0) {}

    GlobalValue(const GlobalValue &) = delete;
    GlobalValue(GlobalValue &&) = default;
    GlobalValue &operator=(const GlobalValue &) = delete;
    GlobalValue &operator=(GlobalValue &&) = default;

    const Symbol &symbol() const {
        return symbol_;
    }

    void setSymbol(Symbol symbol) {
        symbol_ = std::move(symbol);
    }

    const std::string &section() const & {
        return section_;
    }

    std::string &&section() && {
        return std::move(section_);
    }

    void setSection(std::string section) {
        section_ = std::move(section);
    }

    bool isGlobal() const {
        return isGlobal_;
    }

    void setGlobal(bool isGlobal) {
        isGlobal_ = isGlobal;
    }

    int alignment() const {
        return alignment_;
    }

    void setAlignment(int alignment) {
        alignment_ = alignment;
    }

    iterator begin() {
        return iterator(stmts_.begin());
    }

    const_iterator begin() const {
        return const_iterator(stmts_.begin());
    }

    iterator end() {
        return iterator(stmts_.end());
    }

    const_iterator end() const {
        return const_iterator(stmts_.end());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(stmts_.rbegin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(stmts_.rbegin());
    }

    reverse_iterator rend() {
        return reverse_iterator(stmts_.rend());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(stmts_.rend());
    }

    Statement &front() {
        return *begin();
    }

    const Statement &front() const {
        return *begin();
    }

    Statement &back() {
        return *std::prev(end());
    }

    const Statement &back() const {
        return *std::prev(end());
    }

    bool empty() const {
        return stmts_.empty();
    }

    size_t size() const {
        return stmts_.size();
    }

    Statement &add(const_iterator pos, std::unique_ptr<Statement> stmt);

    Statement &prepend(std::unique_ptr<Statement> stmt) {
        return add(begin(), std::move(stmt));
    }

    Statement &append(std::unique_ptr<Statement> stmt) {
        return add(end(), std::move(stmt));
    }

    void remove(const_iterator pos);

    void removeFirst() {
        remove(begin());
    }

    void removeLast() {
        remove(std::prev(end()));
    }

    void clear();

    std::string format() const;

private:
    Symbol symbol_;
    std::string section_;
    bool isGlobal_;
    int alignment_;
    StatementList stmts_;
};

} // namespace mini_llvm::mc

template <typename GlobalValueT>
    requires std::derived_from<GlobalValueT, mini_llvm::mc::GlobalValue>
struct std::formatter<GlobalValueT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const GlobalValueT &G, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", G.format());
    }
};
