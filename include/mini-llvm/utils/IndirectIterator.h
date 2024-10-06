#pragma once

#include <concepts>
#include <iterator>
#include <memory>
#include <utility>

namespace mini_llvm {

template <typename Iterator,
          typename Value = typename std::pointer_traits<typename std::iterator_traits<Iterator>::value_type>::element_type>
class IndirectIterator {
public:
    using iterator_category = std::iterator_traits<Iterator>::iterator_category;
    using value_type = Value;
    using difference_type = std::iterator_traits<Iterator>::difference_type;
    using pointer = Value *;
    using reference = Value &;

    virtual ~IndirectIterator() = default;

    IndirectIterator() = default;
    explicit IndirectIterator(Iterator base) : base_(std::move(base)) {}

    template <typename Iterator2, typename Value2>
        requires std::convertible_to<Iterator2, Iterator>
    IndirectIterator(const IndirectIterator<Iterator2, Value2> &other) : base_(other.base_) {}

    Iterator base() const {
        return base_;
    }

    reference operator*() const {
        return **base_;
    }

    reference operator[](difference_type n) const {
        return *base_[n];
    }

    pointer operator->() const {
        return std::addressof(**base_);
    }

    template <typename Self>
    Self &operator++(this Self &self) {
        ++self.base_;
        return self;
    }

    template <typename Self>
    Self &operator--(this Self &self) {
        --self.base_;
        return self;
    }

    template <typename Self>
    Self operator++(this Self &self, int) {
        return Self(self.base_++);
    }

    template <typename Self>
    Self operator--(this Self &self, int) {
        return Self(self.base_--);
    }

    template <typename Self>
    Self &operator+=(this Self &self, difference_type n) {
        self.base_ += n;
        return self;
    }

    template <typename Self>
    Self &operator-=(this Self &self, difference_type n) {
        self.base_ -= n;
        return self;
    }

    bool operator==(const IndirectIterator &rhs) const {
        return base_ == rhs.base_;
    }

    bool operator!=(const IndirectIterator &rhs) const {
        return base_ != rhs.base_;
    }

    bool operator<(const IndirectIterator &rhs) const {
        return base_ < rhs.base_;
    }

    bool operator>(const IndirectIterator &rhs) const {
        return base_ > rhs.base_;
    }

    bool operator<=(const IndirectIterator &rhs) const {
        return base_ <= rhs.base_;
    }

    bool operator>=(const IndirectIterator &rhs) const {
        return base_ >= rhs.base_;
    }

    difference_type operator-(const IndirectIterator &rhs) const {
        return base_ - rhs.base_;
    }

    template <typename Self>
    Self operator+(this Self self, difference_type n) {
        return self += n;
    }

    template <typename Self>
    Self operator-(this Self self, difference_type n) {
        return self -= n;
    }

private:
    Iterator base_;

    template <typename Iterator2, typename Value2>
    friend class IndirectIterator;
};

} // namespace mini_llvm
