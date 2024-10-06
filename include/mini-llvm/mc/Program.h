#pragma once

#include <format>
#include <iterator>
#include <list>
#include <string>
#include <utility>

#include "mini-llvm/mc/Fragment.h"

namespace mini_llvm::mc {

class Program {
    using FragmentList = std::list<Fragment>;

public:
    using iterator = FragmentList::iterator;
    using const_iterator = FragmentList::const_iterator;
    using reverse_iterator = FragmentList::reverse_iterator;
    using const_reverse_iterator = FragmentList::const_reverse_iterator;

    iterator begin() {
        return fragments_.begin();
    }

    const_iterator begin() const {
        return fragments_.begin();
    }

    iterator end() {
        return fragments_.end();
    }

    const_iterator end() const {
        return fragments_.end();
    }

    reverse_iterator rbegin() {
        return fragments_.rbegin();
    }

    const_reverse_iterator rbegin() const {
        return fragments_.rbegin();
    }

    reverse_iterator rend() {
        return fragments_.rend();
    }

    const_reverse_iterator rend() const {
        return fragments_.rend();
    }

    Fragment &add(const_iterator pos, Fragment fragment) {
        return *fragments_.insert(pos, std::move(fragment));
    }

    Fragment &prepend(Fragment fragment) {
        return add(begin(), std::move(fragment));
    }

    Fragment &append(Fragment fragment) {
        return add(end(), std::move(fragment));
    }

    void remove(const_iterator pos) {
        fragments_.erase(pos);
    }

    void removeFirst() {
        remove(begin());
    }

    void removeLast() {
        remove(std::prev(end()));
    }

    void clear() {
        fragments_.clear();
    }

    std::string format() const;

private:
    FragmentList fragments_;
};

} // namespace mini_llvm::mc

template <>
struct std::formatter<mini_llvm::mc::Program> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const mini_llvm::mc::Program &I, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", I.format());
    }
};
