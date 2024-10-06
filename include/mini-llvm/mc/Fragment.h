#pragma once

#include <cstddef>
#include <format>
#include <iterator>
#include <list>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/mc/Line.h"
#include "mini-llvm/mc/Section.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::mc {

class Fragment {
    using LineList = std::list<std::unique_ptr<Line>>;

public:
    using iterator = IndirectIterator<LineList::iterator, Line>;
    using const_iterator = IndirectIterator<LineList::const_iterator, const Line>;
    using reverse_iterator = IndirectIterator<LineList::reverse_iterator, Line>;
    using const_reverse_iterator = IndirectIterator<LineList::const_reverse_iterator, const Line>;

    Fragment(Section section, bool isGlobal, std::string name)
        : section_(section), isGlobal_(isGlobal), name_(std::move(name)) {}

    Section section() const {
        return section_;
    }

    void setSection(Section section) {
        section_ = section;
    }

    bool isGlobal() const {
        return isGlobal_;
    }

    void setGlobal(bool isGlobal) {
        isGlobal_ = isGlobal;
    }

    const std::string &name() const {
        return name_;
    }

    void setName(std::string name) {
        name_ = std::move(name);
    }

    iterator begin() {
        return iterator(lines_.begin());
    }

    const_iterator begin() const {
        return const_iterator(lines_.begin());
    }

    iterator end() {
        return iterator(lines_.end());
    }

    const_iterator end() const {
        return const_iterator(lines_.end());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(lines_.rbegin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(lines_.rbegin());
    }

    reverse_iterator rend() {
        return reverse_iterator(lines_.rend());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(lines_.rend());
    }

    Line &front() {
        return *begin();
    }

    const Line &front() const {
        return *begin();
    }

    Line &back() {
        return *std::prev(end());
    }

    const Line &back() const {
        return *std::prev(end());
    }

    bool empty() const {
        return lines_.empty();
    }

    size_t size() const {
        return lines_.size();
    }

    Line &add(const_iterator pos, std::unique_ptr<Line> line) {
        return **lines_.insert(pos.base(), std::move(line));
    }

    Line &prepend(std::unique_ptr<Line> line) {
        return add(begin(), std::move(line));
    }

    Line &append(std::unique_ptr<Line> line) {
        return add(end(), std::move(line));
    }

    void remove(const_iterator pos) {
        lines_.erase(pos.base());
    }

    void removeFirst() {
        remove(begin());
    }

    void removeLast() {
        remove(std::prev(end()));
    }

    void clear() {
        lines_.clear();
    }

    std::string format() const;

private:
    Section section_;
    bool isGlobal_;
    std::string name_;
    LineList lines_;
};

} // namespace mini_llvm::mc

template <typename FragmentT>
    requires std::derived_from<FragmentT, mini_llvm::mc::Fragment>
struct std::formatter<FragmentT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const FragmentT &fragment, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", fragment.format());
    }
};
