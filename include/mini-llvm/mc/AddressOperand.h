// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "mini-llvm/mc/Address.h"
#include "mini-llvm/mc/Operand.h"

namespace mini_llvm::mc {

class AddressOperand final : public Operand {
public:
    explicit AddressOperand(Address addr)
        : addr_(std::move(addr)) {}

    explicit AddressOperand(Symbol baseSymbol)
        : addr_(std::move(baseSymbol)) {}

    AddressOperand(Symbol baseSymbol, int64_t offset)
        : addr_(std::move(baseSymbol), offset) {}

    const Address &addr() const & {
        return addr_;
    }

    Address addr() && {
        return std::move(addr_);
    }

    std::string format() const override {
        return addr().format();
    }

private:
    Address addr_;
};

} // namespace mini_llvm::mc
