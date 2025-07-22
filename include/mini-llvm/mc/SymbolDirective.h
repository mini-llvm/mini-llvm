#pragma once

#include <format>
#include <string>
#include <utility>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/mc/Symbol.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT SymbolDirective : public Directive {
public:
    const Symbol &symbol() const {
        return symbol_;
    }

    void setSymbol(Symbol symbol) {
        symbol_ = std::move(symbol);
    }

    std::string format() const override {
        return std::format(".{} {}", directiveName(), symbol());
    }

protected:
    explicit SymbolDirective(Symbol symbol) : symbol_(std::move(symbol)) {}

    virtual std::string directiveName() const = 0;

private:
    Symbol symbol_;
};

} // namespace mini_llvm::mc
