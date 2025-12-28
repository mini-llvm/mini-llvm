// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <string>
#include <utility>

#include "mini-llvm/mc/Symbol.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/HashCombine.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT RISCVAddress {
public:
    explicit RISCVAddress(Symbol baseSymbol)
        : baseSymbol_(std::move(baseSymbol)), offset_(0) {}

    RISCVAddress(Symbol baseSymbol, int64_t offset)
        : baseSymbol_(std::move(baseSymbol)), offset_(offset) {}

    const Symbol &baseSymbol() const & {
        return baseSymbol_;
    }

    Symbol &&baseSymbol() && {
        return std::move(baseSymbol_);
    }

    int64_t offset() const {
        return offset_;
    }

    void setBaseSymbol(Symbol baseSymbol) {
        baseSymbol_ = std::move(baseSymbol);
    }

    void setOffset(int64_t offset) {
        offset_ = offset;
    }

    std::string format() const;

private:
    Symbol baseSymbol_;
    int64_t offset_;
};

} // namespace mini_llvm::mc

template <>
struct std::hash<mini_llvm::mc::RISCVAddress> {
    size_t operator()(const mini_llvm::mc::RISCVAddress &addr) const noexcept {
        size_t seed = 0;

        mini_llvm::hash_combine(seed, addr.baseSymbol());
        mini_llvm::hash_combine(seed, addr.offset());

        return seed;
    }
};

template <>
struct std::formatter<mini_llvm::mc::RISCVAddress> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::mc::RISCVAddress &addr, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", addr.format());
    }
};
