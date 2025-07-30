#pragma once

#include <format>
#include <string>
#include <utility>

#include "mini-llvm/mc/Statement.h"
#include "mini-llvm/mc/Symbol.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT Label final : public Statement {
public:
    explicit Label(Symbol symbol) : symbol_(std::move(symbol)) {}

    const Symbol &symbol() const {
        return symbol_;
    }

    void setSymbol(Symbol symbol) {
        symbol_ = std::move(symbol);
    }

    std::string format() const override {
        return std::format("{}:", symbol());
    }

private:
    Symbol symbol_;
};

} // namespace mini_llvm::mc
