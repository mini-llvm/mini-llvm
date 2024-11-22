#pragma once

#include <utility>

namespace mini_llvm::ir {

enum class Attribute {
#define X(name, specifier) k##name,
#include "mini-llvm/ir/Attribute.def"
#undef X
};

inline constexpr const char *specifier(Attribute attr) {
    using enum Attribute;
    switch (attr) {
#define X(name, specifier) case k##name: return specifier;
#include "mini-llvm/ir/Attribute.def"
#undef X
    default:
        std::unreachable();
    }
}

} // namespace mini_llvm::ir
