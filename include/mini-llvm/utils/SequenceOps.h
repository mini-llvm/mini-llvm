// SPDX-License-Identifier: MIT

#pragma once

#include <utility>

namespace mini_llvm {

namespace detail {

template <typename S>
concept Addable = requires (S &sequence) {
    sequence.push_back(std::declval<typename S::value_type>());
};

template <typename S>
    requires Addable<S>
void add(S &sequence, typename S::value_type value) {
    sequence.push_back(std::move(value));
}

} // namespace detail

namespace sequence_ops {

template <typename S>
    requires detail::Addable<S>
S &operator+=(S &sequence, typename S::value_type value) {
    detail::add(sequence, std::move(value));
    return sequence;
}

template <typename S>
    requires detail::Addable<S>
S operator+=(S &&sequence, typename S::value_type value) {
    detail::add(sequence, std::move(value));
    return std::move(sequence);
}

template <typename S>
    requires detail::Addable<S>
S &operator,(S &sequence, typename S::value_type value) {
    return sequence += std::move(value);
}

template <typename S>
    requires detail::Addable<S>
S operator,(S &&sequence, typename S::value_type value) {
    return std::move(sequence) += std::move(value);
}

template <typename S>
    requires detail::Addable<S>
S operator+(S sequence, typename S::value_type value) {
    return std::move(sequence) += std::move(value);
}

} // namespace sequence_ops

} // namespace mini_llvm
