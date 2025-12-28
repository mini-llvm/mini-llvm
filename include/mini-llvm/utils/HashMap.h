// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <functional>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>

#include "mini-llvm/utils/OptionalRef.h"

namespace mini_llvm {

template <typename Key,
          typename Value,
          typename Hash = std::hash<Key>,
          typename Equal = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, Value>>>
class HashMap : private std::unordered_map<Key, Value, Hash, Equal, Allocator> {
    using Base = std::unordered_map<Key, Value, Hash, Equal, Allocator>;

public:
    using typename Base::key_type;
    using typename Base::mapped_type;
    using typename Base::value_type;
    using typename Base::size_type;
    using typename Base::difference_type;
    using typename Base::hasher;
    using typename Base::key_equal;
    using typename Base::allocator_type;
    using typename Base::reference;
    using typename Base::const_reference;
    using typename Base::pointer;
    using typename Base::const_pointer;
    using typename Base::iterator;
    using typename Base::const_iterator;
    using typename Base::local_iterator;
    using typename Base::const_local_iterator;
    using typename Base::node_type;
    using typename Base::insert_return_type;

    using Base::Base;
    using Base::operator=;
    using Base::begin;
    using Base::end;
    using Base::cbegin;
    using Base::cend;
    using Base::empty;
    using Base::size;
    using Base::max_size;
    using Base::contains;
    using Base::count;
    using Base::at;
    using Base::find;
    using Base::equal_range;
    using Base::insert;
    using Base::insert_or_assign;
    using Base::emplace;
    using Base::emplace_hint;
    using Base::try_emplace;
    using Base::erase;
    using Base::clear;
    using Base::extract;
    using Base::merge;
    using Base::swap;
    using Base::bucket_count;
    using Base::max_bucket_count;
    using Base::bucket;
    using Base::bucket_size;
    using Base::load_factor;
    using Base::max_load_factor;
    using Base::rehash;
    using Base::reserve;
    using Base::hash_function;
    using Base::key_eq;
    using Base::get_allocator;

    template <typename Key2 = Key>
    Value &operator[](const Key2 &key) {
        auto i = find(key);
        assert(i != end());
        return i->second;
    }

    template <typename Key2 = Key>
    const Value &operator[](const Key2 &key) const {
        auto i = find(key);
        assert(i != end());
        return i->second;
    }

    template <typename Key2 = Key>
    OptionalRef<Value> get(const Key2 &key) {
        auto i = find(key);
        if (i != end()) {
            return i->second;
        }
        return std::nullopt;
    }

    template <typename Key2 = Key>
    OptionalRef<const Value> get(const Key2 &key) const {
        auto i = find(key);
        if (i != end()) {
            return i->second;
        }
        return std::nullopt;
    }

    template <typename Key2 = Key, typename Value2 = Value>
    std::pair<iterator, bool> put(Key2 &&key, Value2 &&value) {
        return insert_or_assign(std::forward<Key2>(key), std::forward<Value2>(value));
    }
};

} // namespace mini_llvm
