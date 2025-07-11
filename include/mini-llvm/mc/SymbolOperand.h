#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/mc/Symbol.h"

namespace mini_llvm::mc {

class SymbolOperand final : public Operand {
public:
    explicit SymbolOperand(Symbol symbol) : symbol_(std::move(symbol)) {}

    const Symbol &symbol() const {
        return symbol_;
    }

    void setSymbol(Symbol symbol) {
        symbol_ = std::move(symbol);
    }

    std::string format() const override {
        return symbol().format();
    }

private:
    Symbol symbol_;
};

} // namespace mini_llvm::mc
