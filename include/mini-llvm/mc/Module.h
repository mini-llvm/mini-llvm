// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <iterator>
#include <list>
#include <string>
#include <utility>

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT Module {
    using GlobalValueList = std::list<GlobalValue>;

public:
    using iterator = GlobalValueList::iterator;
    using const_iterator = GlobalValueList::const_iterator;
    using reverse_iterator = GlobalValueList::reverse_iterator;
    using const_reverse_iterator = GlobalValueList::const_reverse_iterator;

    Module() = default;
    Module(const Module &) = delete;
    Module(Module &&) = default;
    Module &operator=(const Module &) = delete;
    Module &operator=(Module &&) = default;

    iterator begin() {
        return globalValues_.begin();
    }

    const_iterator begin() const {
        return globalValues_.begin();
    }

    iterator end() {
        return globalValues_.end();
    }

    const_iterator end() const {
        return globalValues_.end();
    }

    reverse_iterator rbegin() {
        return globalValues_.rbegin();
    }

    const_reverse_iterator rbegin() const {
        return globalValues_.rbegin();
    }

    reverse_iterator rend() {
        return globalValues_.rend();
    }

    const_reverse_iterator rend() const {
        return globalValues_.rend();
    }

    GlobalValue &add(const_iterator pos, GlobalValue G);

    GlobalValue &prepend(GlobalValue G) {
        return add(begin(), std::move(G));
    }

    GlobalValue &append(GlobalValue G) {
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

    std::string format() const;

private:
    GlobalValueList globalValues_;
};

} // namespace mini_llvm::mc

template <>
struct std::formatter<mini_llvm::mc::Module> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::mc::Module &I, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", I.format());
    }
};
