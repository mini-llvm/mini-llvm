#pragma once

#include <functional>
#include <unordered_map>
#include <utility>

namespace mini_llvm {

template <typename K,
          typename V,
          typename Hash = std::hash<K>,
          typename Equal = std::equal_to<K>,
          typename Allocator = std::allocator<std::pair<const K, V>>>
class HashMap : private std::unordered_map<K, V, Hash, Equal, Allocator> {
    using Base = std::unordered_map<K, V, Hash, Equal, Allocator>;

    template <typename T>
    class [[nodiscard]] Proxy {
    public:
        Proxy(const Proxy &) = delete;
        Proxy(Proxy &&) = delete;

        Proxy &operator=(const Proxy &) = delete;
        Proxy &operator=(Proxy &&) = delete;

        template <typename U = V>
        V &operator=(U &&value) && {
            return parent_.insert_or_assign(std::forward<T>(key_), std::forward<U>(value)).first->second;
        }

    private:
        HashMap &parent_;
        T &&key_;

        Proxy(HashMap &parent, T &&key) : parent_(parent), key_(std::forward<T>(key)) {}

        friend class HashMap;
    };

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

    template <typename T = K>
    V &operator[](const T &key) {
        return find(key)->second;
    }

    template <typename T = K>
    const V &operator[](const T &key) const {
        return find(key)->second;
    }

    template <typename T = K>
    Proxy<T> operator()(T &&key) {
        return Proxy<T>(*this, std::forward<T>(key));
    }
};

} // namespace mini_llvm
