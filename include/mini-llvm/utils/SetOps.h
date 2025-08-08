// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace mini_llvm {

namespace detail {

template <typename T>
concept SetImpl = requires {
    requires std::forward_iterator<typename T::iterator>;
    requires std::forward_iterator<typename T::const_iterator>;
    requires std::same_as<typename std::iterator_traits<typename T::iterator>::value_type, typename T::value_type>;
    requires std::same_as<typename std::iterator_traits<typename T::const_iterator>::value_type, typename T::value_type>;
    { std::declval<T &>().begin() } -> std::same_as<typename T::iterator>;
    { std::declval<T &>().end() } -> std::same_as<typename T::iterator>;
    { std::declval<const T &>().begin() } -> std::same_as<typename T::const_iterator>;
    { std::declval<const T &>().end() } -> std::same_as<typename T::const_iterator>;
    { std::declval<T &>().contains(std::declval<const typename T::value_type &>()) } -> std::convertible_to<bool>;
    { std::declval<const T &>().contains(std::declval<const typename T::value_type &>()) } -> std::convertible_to<bool>;
    std::declval<T &>().insert(std::declval<const typename T::value_type &>());
    std::declval<T &>().erase(std::declval<const typename T::value_type &>());
    { std::declval<T &>().erase(std::declval<const typename T::iterator &>()) } -> std::same_as<typename T::iterator>;
    { std::declval<T &>().erase(std::declval<const typename T::const_iterator &>()) } -> std::same_as<typename T::iterator>;
};

template <typename T>
concept Set = SetImpl<std::remove_cv_t<std::remove_reference_t<T>>>;

} // namespace detail

namespace set_ops {

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T>
S &operator|=(S &lhs, const T &rhs) {
    for (const auto &value : rhs) {
        lhs.insert(value);
    }
    return lhs;
}

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T> && std::is_rvalue_reference_v<T &&>
S &operator|=(S &lhs, T &&rhs) {
    for (auto &value : rhs) {
        lhs.insert(std::move(value));
    }
    return lhs;
}

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T>
S &operator&=(S &lhs, const T &rhs) {
    for (auto i = lhs.begin(); i != lhs.end();) {
        if (!rhs.contains(*i)) {
            i = lhs.erase(i);
        } else {
            ++i;
        }
    }
    return lhs;
}

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T>
S &operator-=(S &lhs, const T &rhs) {
    for (const auto &element : rhs) {
        lhs.erase(element);
    }
    return lhs;
}

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T>
S operator|(S lhs, const T &rhs) {
    return lhs |= rhs;
}

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T> && std::is_rvalue_reference_v<T &&>
S operator|(S lhs, T &&rhs) {
    return lhs |= std::move(rhs);
}

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T>
S operator&(S lhs, const T &rhs) {
    return lhs &= rhs;
}

template <typename S, typename T>
    requires detail::Set<S> && detail::Set<T>
S operator-(S lhs, const T &rhs) {
    return lhs -= rhs;
}

} // namespace set_ops

} // namespace mini_llvm
