// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/targets/riscv/mc/RISCVAddress.h"

namespace mini_llvm::mc {

class RISCVAddressOperand final : public Operand {
public:
    explicit RISCVAddressOperand(RISCVAddress addr)
        : addr_(std::move(addr)) {}

    explicit RISCVAddressOperand(Symbol baseSymbol)
        : addr_(std::move(baseSymbol)) {}

    RISCVAddressOperand(Symbol baseSymbol, int64_t offset)
        : addr_(std::move(baseSymbol), offset) {}

    const RISCVAddress &addr() const & {
        return addr_;
    }

    RISCVAddress &&addr() && {
        return std::move(addr_);
    }

    std::string format() const override {
        return addr().format();
    }

private:
    RISCVAddress addr_;
};

} // namespace mini_llvm::mc
