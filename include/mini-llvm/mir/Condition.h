// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>

namespace mini_llvm::mir {

enum class Condition {
#define X(name, specifier, inverted) k##name,
#include "mini-llvm/mir/Condition.def"
#undef X
};

inline constexpr const char *specifier(Condition cond) {
    switch (cond) {
#define X(name, specifier, inverted) case Condition::k##name: return specifier;
#include "mini-llvm/mir/Condition.def"
#undef X
    default:
        abort();
    }
}

inline constexpr Condition inverted(Condition cond) {
    switch (cond) {
#define X(name, specifier, inverted) case Condition::k##name: return Condition::k##inverted;
#include "mini-llvm/mir/Condition.def"
#undef X
    default:
        abort();
    }
}

} // namespace mini_llvm::mir
