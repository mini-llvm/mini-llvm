// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <functional>
#include <string>

#include "mini-llvm/utils/HashCombine.h"

namespace mini_llvm::ir {

struct Symbol {
    enum class Scope {
        kGlobal,
        kLocal,
    };

    Scope scope;
    std::string name;
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
    return lhs.scope == rhs.scope && lhs.name == rhs.name;
}

} // namespace mini_llvm::ir

template <>
struct std::hash<mini_llvm::ir::Symbol> {
    constexpr size_t operator()(const mini_llvm::ir::Symbol &symbol) const noexcept {
        size_t seed = 0;

        mini_llvm::hash_combine(seed, symbol.scope);
        mini_llvm::hash_combine(seed, symbol.name);

        return seed;
    }
};
