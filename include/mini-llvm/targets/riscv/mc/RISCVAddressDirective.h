// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <format>
#include <string>
#include <utility>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/mc/Symbol.h"
#include "mini-llvm/targets/riscv/mc/RISCVAddress.h"

namespace mini_llvm::mc {

class RISCVAddressDirective final : public Directive {
public:
    explicit RISCVAddressDirective(RISCVAddress addr)
        : addr_(std::move(addr)) {}

    explicit RISCVAddressDirective(Symbol baseSymbol)
        : addr_(std::move(baseSymbol)) {}

    RISCVAddressDirective(Symbol baseSymbol, int64_t offset)
        : addr_(std::move(baseSymbol), offset) {}

    const RISCVAddress &addr() const & {
        return addr_;
    }

    RISCVAddress &&addr() && {
        return std::move(addr_);
    }

    void setAddr(RISCVAddress addr) {
        addr_ = std::move(addr);
    }

    std::string format() const override {
        return std::format(".dword {}", addr());
    }

private:
    RISCVAddress addr_;
};

} // namespace mini_llvm::mc
