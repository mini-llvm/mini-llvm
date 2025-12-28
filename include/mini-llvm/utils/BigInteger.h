// SPDX-License-Identifier: MIT

#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT BigInteger {
public:
    BigInteger() noexcept;
    BigInteger(int64_t value);
    explicit BigInteger(std::string_view str, int base = 10);

    BigInteger(const BigInteger &other) = default;
    BigInteger &operator=(const BigInteger &other) = default;

    BigInteger(BigInteger &&other) noexcept = default;
    BigInteger &operator=(BigInteger &&other) noexcept = default;

    BigInteger operator+() const &;
    BigInteger operator+() &&;
    BigInteger operator-() const &;
    BigInteger operator-() &&;

    BigInteger &operator+=(const BigInteger &lhs);
    BigInteger &operator-=(const BigInteger &lhs);
    BigInteger &operator*=(const BigInteger &lhs);
    BigInteger &operator/=(int32_t lhs);

    BigInteger &operator++();
    BigInteger operator++(int);
    BigInteger &operator--();
    BigInteger operator--(int);

    int64_t toInt64() const noexcept;
    std::string toString(int base = 10) const;

    size_t hashCode() const noexcept;

    static std::optional<BigInteger> parse(std::string_view str, int base = 10);

private:
    int sign_{};
    std::vector<uint32_t> digits_;

    BigInteger(int sign, std::vector<uint32_t> digits);

    friend MINI_LLVM_EXPORT bool operator==(const BigInteger &lhs, const BigInteger &rhs) noexcept;
    friend MINI_LLVM_EXPORT std::strong_ordering operator<=>(const BigInteger &lhs, const BigInteger &rhs) noexcept;

    friend MINI_LLVM_EXPORT BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs);
    friend MINI_LLVM_EXPORT BigInteger operator-(const BigInteger &lhs, const BigInteger &rhs);
    friend MINI_LLVM_EXPORT BigInteger operator*(const BigInteger &lhs, const BigInteger &rhs);
    friend MINI_LLVM_EXPORT BigInteger operator/(const BigInteger &lhs, int32_t rhs);
    friend MINI_LLVM_EXPORT int32_t operator%(const BigInteger &lhs, int32_t rhs);

    friend MINI_LLVM_EXPORT BigInteger abs(const BigInteger &value);
    friend MINI_LLVM_EXPORT BigInteger abs(BigInteger &&value);
    friend MINI_LLVM_EXPORT std::pair<BigInteger, int32_t> divRem(const BigInteger &lhs, int32_t rhs);
};

MINI_LLVM_EXPORT bool operator==(const BigInteger &lhs, const BigInteger &rhs) noexcept;
MINI_LLVM_EXPORT std::strong_ordering operator<=>(const BigInteger &lhs, const BigInteger &rhs) noexcept;

MINI_LLVM_EXPORT BigInteger operator+(const BigInteger &lhs, const BigInteger &rhs);
MINI_LLVM_EXPORT BigInteger operator-(const BigInteger &lhs, const BigInteger &rhs);
MINI_LLVM_EXPORT BigInteger operator*(const BigInteger &lhs, const BigInteger &rhs);
MINI_LLVM_EXPORT BigInteger operator/(const BigInteger &lhs, int32_t rhs);
MINI_LLVM_EXPORT int32_t operator%(const BigInteger &lhs, int32_t rhs);

MINI_LLVM_EXPORT BigInteger abs(const BigInteger &value);
MINI_LLVM_EXPORT BigInteger abs(BigInteger &&value);
MINI_LLVM_EXPORT std::pair<BigInteger, int32_t> divRem(const BigInteger &lhs, int32_t rhs);

} // namespace mini_llvm

template <>
struct std::hash<mini_llvm::BigInteger> {
    size_t operator()(const mini_llvm::BigInteger &value) const noexcept {
        return value.hashCode();
    }
};

template <>
struct std::formatter<mini_llvm::BigInteger> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const mini_llvm::BigInteger &value, FormatContext &ctx) const {
        return std::format(ctx.out(), "{}", value.toString());
    }
};
