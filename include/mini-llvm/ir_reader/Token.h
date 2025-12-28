// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>
#include <variant>
#include <vector>

#include "mini-llvm/utils/BigInteger.h"

namespace mini_llvm::ir {

struct Token {
    enum class Kind {
#define X(name) k##name,
#include "mini-llvm/ir_reader/TokenKind.def"
#undef X
    };

    Kind kind;
    std::variant<std::monostate, BigInteger, std::string, std::vector<int8_t>> value;
    const char *location;
};

inline bool operator==(const Token &lhs, const Token &rhs) {
    return lhs.kind == rhs.kind && lhs.value == rhs.value && lhs.location == rhs.location;
}

inline constexpr const char *name(Token::Kind kind) {
    switch (kind) {
#define X(name) case Token::Kind::k##name: return #name;
#include "mini-llvm/ir_reader/TokenKind.def"
#undef X
    default:
        abort();
    }
}

} // namespace mini_llvm::ir
