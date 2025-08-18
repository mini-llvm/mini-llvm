// SPDX-License-Identifier: MIT

#pragma once

#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/ir_reader/Symbol.h"
#include "mini-llvm/ir_reader/Token.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class ParseException : public std::exception {
public:
    ParseException(std::string message, std::vector<Token>::const_iterator location)
        : message_(std::move(message)), location_(location) {}

    const std::string &message() const & {
        return message_;
    }

    std::string &&message() && {
        return std::move(message_);
    }

    std::vector<Token>::const_iterator location() const {
        return location_;
    }

    const char *what() const noexcept override {
        return "ParseException";
    }

private:
    std::string message_;
    std::vector<Token>::const_iterator location_;
};

class MINI_LLVM_EXPORT Parser {
public:
    explicit Parser(std::vector<Token>::const_iterator current);

    ~Parser();

    Parser(Parser &&) noexcept;
    Parser &operator=(Parser &&) noexcept;

    Module parseModule();
    std::shared_ptr<GlobalVar> parseGlobalVarHeader(bool &isDeclaration);
    void parseGlobalVarInitializer(GlobalVar &G);
    std::shared_ptr<Function> parseFunctionHeader(bool &isDeclaration);
    void parseFunctionBody(Function &F);
    void parseBasicBlock(BasicBlock &B);
    std::shared_ptr<Instruction> parseInstruction();
    std::shared_ptr<Value> parseValue(const Type &type);
    std::shared_ptr<Value> parseIdentifier(const Type &type);
    std::shared_ptr<Constant> parseConstant(const Type &type);
    std::unique_ptr<Type> parseType();
    Symbol parseSymbol(std::optional<Symbol::Scope> scope = std::nullopt);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

MINI_LLVM_EXPORT Module parseModule(const std::vector<Token> &tokens);

} // namespace mini_llvm::ir
