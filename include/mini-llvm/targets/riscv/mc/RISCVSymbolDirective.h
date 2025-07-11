#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/Symbol.h"
#include "mini-llvm/mc/SymbolDirective.h"

namespace mini_llvm::mc {

class RISCVSymbolDirective final : public SymbolDirective {
public:
    explicit RISCVSymbolDirective(Symbol symbol)
        : SymbolDirective(std::move(symbol)) {}

protected:
    std::string directiveName() const override {
        return "dword";
    }
};

} // namespace mini_llvm::mc
