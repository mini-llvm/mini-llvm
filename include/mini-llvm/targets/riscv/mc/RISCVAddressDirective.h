// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mc/AddressDirective.h"
#include "mini-llvm/mc/Symbol.h"

namespace mini_llvm::mc {

class RISCVAddressDirective final : public AddressDirective {
public:
    explicit RISCVAddressDirective(Address addr)
        : AddressDirective(std::move(addr)) {}

    explicit RISCVAddressDirective(Symbol baseSymbol)
        : AddressDirective(std::move(baseSymbol)) {}

    RISCVAddressDirective(Symbol baseSymbol, int64_t offset)
        : AddressDirective(std::move(baseSymbol), offset) {}

protected:
    std::string directiveName() const override {
        return "dword";
    }
};

} // namespace mini_llvm::mc
