#pragma once

#include <set>
#include <type_traits>
#include <unordered_set>

namespace mini_llvm {

namespace detail {

template <typename>
struct is_std_set_impl : std::false_type {};

template <typename Key, typename Compare, typename Allocator>
struct is_std_set_impl<std::set<Key, Compare, Allocator>> : std::true_type {};

template <typename>
struct is_std_unordered_set_impl : std::false_type {};

template <typename Key, typename Hash, typename Pred, typename Allocator>
struct is_std_unordered_set_impl<std::unordered_set<Key, Hash, Pred, Allocator>> : std::true_type {};

template <typename T>
struct is_std_set : detail::is_std_set_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_set_v = is_std_set<T>::value;

template <typename T>
struct is_std_unordered_set : detail::is_std_unordered_set_impl<std::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_std_unordered_set_v = is_std_unordered_set<T>::value;

template <typename T>
concept Set = is_std_set_v<T> || is_std_unordered_set_v<T>;

} // namespace detail

namespace set_ops {

template <typename S>
    requires detail::Set<S>
S &operator|=(S &lhs, const S &rhs) {
    lhs.insert(rhs.begin(), rhs.end());
    return lhs;
}

template <typename S>
    requires detail::Set<S>
S &operator&=(S &lhs, const S &rhs) {
    for (auto i = lhs.begin(); i != lhs.end();) {
        if (!rhs.contains(*i)) {
            i = lhs.erase(i);
        } else {
            ++i;
        }
    }
    return lhs;
}

template <typename S>
    requires detail::Set<S>
S &operator-=(S &lhs, const S &rhs) {
    for (const auto &element : rhs) {
        lhs.erase(element);
    }
    return lhs;
}

template <typename S>
    requires detail::Set<S>
S operator|(S lhs, const S &rhs) {
    return lhs |= rhs;
}

template <typename S>
    requires detail::Set<S>
S operator&(S lhs, const S &rhs) {
    return lhs &= rhs;
}

template <typename S>
    requires detail::Set<S>
S operator-(S lhs, const S &rhs) {
    return lhs -= rhs;
}

} // namespace set_ops

} // namespace mini_llvm
