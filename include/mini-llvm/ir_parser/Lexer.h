#pragma once

#include <exception>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/ir_parser/Token.h"

namespace mini_llvm::ir {

class LexException : public std::exception {
public:
    LexException(const char *location, std::string message)
        : location_(location), message_(std::move(message)) {}

    const char *location() const {
        return location_;
    }

    const std::string &message() const {
        return message_;
    }

    const char *what() const noexcept override {
        return "LexException";
    }

private:
    const char *location_;
    std::string message_;
};

class Lexer {
public:
    explicit Lexer(const char *cursor) : cursor_(cursor) {}
    std::optional<Token> lastToken();
    Token nextToken();
    explicit operator std::vector<Token>();

private:
    const char *cursor_;
    std::optional<Token> lastToken_;

    Token nextTokenImpl();
};

} // namespace mini_llvm::ir
