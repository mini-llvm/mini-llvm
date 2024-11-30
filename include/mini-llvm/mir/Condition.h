#pragma once

#include "mini-llvm/utils/Panic.h"

namespace mini_llvm::mir {

enum class Condition {
#define X(name, specifier) k##name,
#include "mini-llvm/mir/Condition.def"
#undef X
};

inline constexpr const char *specifier(Condition cond) {
    switch (cond) {
#define X(name, specifier) case Condition::k##name: return specifier;
#include "mini-llvm/mir/Condition.def"
#undef X
    default:
        panic();
    }
}

} // namespace mini_llvm::mir
