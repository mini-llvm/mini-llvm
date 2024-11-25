#pragma once

#include <climits>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <tuple>
#include <utility>

namespace mini_llvm {

// https://www.boost.org/doc/libs/1_86_0/libs/container_hash/doc/html/hash.html

template <typename T>
constexpr void hash_combine(size_t &seed, const T &v) noexcept;

template <typename T>
constexpr size_t hash_value(const T &v) noexcept {
    return std::hash<T>()(v);
}

template <typename T1, typename T2>
constexpr size_t hash_value(const std::pair<T1, T2> &v) noexcept {
    size_t seed = 0;

    hash_combine(seed, v.first);
    hash_combine(seed, v.second);

    return seed;
}

namespace detail {

template <size_t I, typename T>
constexpr void hash_combine_tuple(size_t &, const T &) noexcept {}

template <size_t I, typename T>
    requires (I < std::tuple_size_v<T>)
constexpr void hash_combine_tuple(size_t &seed, const T &v) noexcept {
    hash_combine(seed, std::get<I>(v));
    hash_combine_tuple<I + 1>(seed, v);
}

} // namespace detail

template <typename... Types>
constexpr size_t hash_value(const std::tuple<Types...> &v) noexcept {
    size_t seed = 0;

    detail::hash_combine_tuple<0>(seed, v);

    return seed;
}

namespace detail {

template <size_t>
struct hash_mix_impl;

template <>
struct hash_mix_impl<64> {
    static constexpr uint64_t fn(uint64_t x) noexcept {
        constexpr uint64_t m = 0xe9846af9b1a615d;

        x ^= x >> 32;
        x *= m;
        x ^= x >> 32;
        x *= m;
        x ^= x >> 28;

        return x;
    }
};

template <>
struct hash_mix_impl<32> {
    static constexpr uint32_t fn(uint32_t x) noexcept {
        constexpr uint32_t m1 = 0x21f0aaad;
        constexpr uint32_t m2 = 0x735a2d97;

        x ^= x >> 16;
        x *= m1;
        x ^= x >> 15;
        x *= m2;
        x ^= x >> 15;

        return x;
    }
};

inline constexpr size_t hash_mix(size_t v) noexcept {
    return hash_mix_impl<sizeof(size_t) * CHAR_BIT>::fn(v);
}

} // namespace detail

template <typename T>
constexpr void hash_combine(size_t &seed, const T &v) noexcept {
    seed = detail::hash_mix(seed + 0x9e3779b9 + hash_value(v));
}

template <typename T>
struct Hash {
    constexpr size_t operator()(const T &v) const noexcept {
        return hash_value(v);
    }
};

} // namespace mini_llvm
