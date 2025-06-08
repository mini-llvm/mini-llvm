#pragma once

#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/ir_reader/Token.h"

namespace mini_llvm::ir {

class LexException : public std::exception {
public:
    LexException(std::string message, const char *location)
        : message_(std::move(message)), location_(location) {}

    const std::string &message() const {
        return message_;
    }

    const char *location() const {
        return location_;
    }

    const char *what() const noexcept override {
        return "LexException";
    }

private:
    std::string message_;
    const char *location_;
};

class Lexer {
public:
    explicit Lexer(const char *current);
    ~Lexer();

    std::optional<Token> lastToken();
    Token nextToken();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

std::vector<Token> lex(const char *source);

} // namespace mini_llvm::ir
