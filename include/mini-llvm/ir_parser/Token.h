#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "mini-llvm/utils/Panic.h"

namespace mini_llvm::ir {

struct Token {
    enum class Kind {
#define X(name) k##name,
#include "mini-llvm/ir_parser/TokenKind.def"
#undef X
    };

    Kind kind;
    std::variant<std::monostate, int64_t, std::string, std::vector<int8_t>> value;
    const char *location;
};

inline bool operator==(const Token &lhs, const Token &rhs) {
    return lhs.kind == rhs.kind && rhs.value == rhs.value && lhs.location == rhs.location;
}

inline constexpr const char *name(Token::Kind kind) {
    switch (kind) {
#define X(name) case Token::Kind::k##name: return #name;
#include "mini-llvm/ir_parser/TokenKind.def"
#undef X
    default:
        panic();
    }
}

} // namespace mini_llvm::ir
