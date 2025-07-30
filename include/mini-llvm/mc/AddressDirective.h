#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <utility>

#include "mini-llvm/mc/Address.h"
#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/mc/Symbol.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT AddressDirective : public Directive {
public:
    const Address &addr() const & {
        return addr_;
    }

    Address addr() && {
        return std::move(addr_);
    }

    std::string format() const override {
        return std::format(".{} {}", directiveName(), addr());
    }

protected:
    explicit AddressDirective(Address addr)
        : addr_(std::move(addr)) {}

    explicit AddressDirective(Symbol baseSymbol)
        : addr_(std::move(baseSymbol)) {}

    AddressDirective(Symbol baseSymbol, int64_t offset)
        : addr_(std::move(baseSymbol), offset) {}

    virtual std::string directiveName() const = 0;

private:
    Address addr_;
};

} // namespace mini_llvm::mc
