// SPDX-License-Identifier: MIT

#pragma once

#include <climits>
#include <cstddef>
#include <cstdint>
#include <functional>

namespace mini_llvm {

// https://www.boost.org/doc/libs/1_86_0/libs/container_hash/doc/html/hash.html

// Copyright 2005-2014 Daniel James.
// Copyright 2021, 2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// Modified under the MIT License.

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

template <typename T, typename Hash = std::hash<T>>
constexpr void hash_combine(size_t &seed, const T &v) noexcept {
    seed = detail::hash_mix(seed + 0x9e3779b9 + Hash()(v));
}

} // namespace mini_llvm
