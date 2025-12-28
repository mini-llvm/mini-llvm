// SPDX-License-Identifier: MIT

#include "mini-llvm/utils/BigInteger.h"

#include <algorithm>
#include <bit>
#include <cassert>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "mini-llvm/utils/HashCombine.h"

using namespace mini_llvm;

namespace {

constexpr char kDigits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

constexpr int kDigitValues[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
     0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
    -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

uint32_t uabs(int32_t value) {
    if (value >= 0) {
        return static_cast<uint32_t>(value);
    } else {
        return -static_cast<uint32_t>(value);
    }
}

std::strong_ordering cmpImpl(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) {
    size_t n = a.size(), m = b.size();
    if (n != m) {
        return n <=> m;
    }
    for (size_t i = n; i-- > 0;) {
        if (a[i] != b[i]) {
            return a[i] <=> b[i];
        }
    }
    return std::strong_ordering::equal;
}

std::vector<uint32_t> addImpl(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) {
    size_t n = a.size(), m = b.size();
    std::vector<uint32_t> c(std::max(n, m) + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < std::max(n, m); ++i) {
        uint64_t sum = carry;
        if (i < n) {
            sum += a[i];
        }
        if (i < m) {
            sum += b[i];
        }
        c[i] = sum & 0xffffffff;
        carry = sum >> 32;
    }
    if (carry) {
        c[std::max(n, m)] = carry;
    }
    while (!c.empty() && c.back() == 0) {
        c.pop_back();
    }
    return c;
}

std::vector<uint32_t> subImpl(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) {
    size_t n = a.size(), m = b.size();
    std::vector<uint32_t> c(n);
    int64_t borrow = 0;
    for (size_t i = 0; i < n; ++i) {
        int64_t diff = a[i] - borrow;
        if (i < m) {
            diff -= b[i];
        }
        c[i] = diff & 0xffffffff;
        borrow = diff < 0;
    }
    assert(borrow == 0);
    while (!c.empty() && c.back() == 0) {
        c.pop_back();
    }
    return c;
}

std::vector<uint32_t> mulImpl(const std::vector<uint32_t> &a, const std::vector<uint32_t> &b) {
    size_t n = a.size(), m = b.size();
    std::vector<uint32_t> c(n + m);
    for (size_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < m; ++j) {
            uint64_t prod = c[i + j] + carry + static_cast<uint64_t>(a[i]) * static_cast<uint64_t>(b[j]);
            c[i + j] = prod & 0xffffffff;
            carry = prod >> 32;
        }
        if (carry) {
            c[i + m] = carry;
        }
    }
    while (!c.empty() && c.back() == 0) {
        c.pop_back();
    }
    return c;
}

std::pair<std::vector<uint32_t>, uint32_t> divRemImpl(const std::vector<uint32_t> &a, uint32_t b) {
    assert(b != 0);
    size_t n = a.size();
    std::vector<uint32_t> q(n);
    uint64_t r = 0;
    for (size_t i = n; i-- > 0;) {
        r = (r << 32) | a[i];
        q[i] = r / b;
        r %= b;
    }
    while (!q.empty() && q.back() == 0) {
        q.pop_back();
    }
    return {std::move(q), static_cast<uint32_t>(r)};
}

} // namespace

BigInteger::BigInteger() noexcept = default;

BigInteger::BigInteger(int64_t value) {
    if (value == 0) {
        return;
    }
    sign_ = 1;
    if (value < 0) {
        sign_ = -sign_;
        value = std::bit_cast<int64_t>(-static_cast<uint64_t>(value));
    }
    while (value > 0) {
        digits_.push_back(value & 0xffffffff);
        value >>= 32;
    }
}

BigInteger::BigInteger(std::string_view str, int base) {
    std::optional<BigInteger> value = parse(str, base);
    assert(value);
    *this = *std::move(value);
}

BigInteger::BigInteger(int sign, std::vector<uint32_t> digits)
    : sign_(sign), digits_(std::move(digits)) {}

BigInteger BigInteger::operator+() const & {
    return *this;
}

BigInteger BigInteger::operator+() && {
    return std::move(*this);
}

BigInteger BigInteger::operator-() const & {
    return BigInteger(-sign_, digits_);
}

BigInteger BigInteger::operator-() && {
    return BigInteger(-sign_, std::move(digits_));
}

BigInteger &BigInteger::operator+=(const BigInteger &rhs) {
    *this = *this + rhs;
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &rhs) {
    *this = *this - rhs;
    return *this;
}

BigInteger &BigInteger::operator*=(const BigInteger &rhs) {
    *this = *this * rhs;
    return *this;
}

BigInteger &BigInteger::operator/=(int32_t rhs) {
    *this = *this / rhs;
    return *this;
}

BigInteger &BigInteger::operator++() {
    *this += 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger tmp = *this;
    *this += 1;
    return tmp;
}

BigInteger &BigInteger::operator--() {
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger tmp = *this;
    *this -= 1;
    return tmp;
}

int64_t BigInteger::toInt64() const noexcept {
    uint64_t value = 0;
    size_t n = digits_.size();
    for (size_t i = n; i-- > 0;) {
        value = (value << 32) | digits_[i];
    }
    if (sign_ < 0) {
        value = -value;
    }
    return std::bit_cast<int64_t>(value);
}

std::string BigInteger::toString(int base) const {
    BigInteger q = abs(*this);
    int32_t r;
    std::string str;
    while (q > 0) {
        std::tie(q, r) = divRem(q, base);
        str.push_back(kDigits[r]);
    }
    if (str.empty()) {
        str.push_back(kDigits[0]);
    }
    if (*this < 0) {
        str.push_back('-');
    }
    std::reverse(str.begin(), str.end());
    return str;
}

size_t BigInteger::hashCode() const noexcept {
    size_t seed = 0;
    hash_combine(seed, sign_);
    for (uint32_t digit : digits_) {
        hash_combine(seed, digit);
    }
    return seed;
}

std::optional<BigInteger> BigInteger::parse(std::string_view str, int base) {
    assert(2 <= base && base <= 36);
    size_t n = str.size();
    size_t i = 0;
    bool isNegative = false;
    if (i == n) {
        return std::nullopt;
    }
    if (str[i] == '-') {
        isNegative = true;
        ++i;
    }
    BigInteger value;
    if (i == n) {
        return std::nullopt;
    }
    while (i < n) {
        int digitValue = kDigitValues[static_cast<uint8_t>(str[i])];
        if (!(0 <= digitValue && digitValue < base)) {
            return std::nullopt;
        }
        value = value * base + digitValue;
        ++i;
    }
    if (isNegative) {
        value = -value;
    }
    return value;
}

bool mini_llvm::operator==(const BigInteger &lhs, const BigInteger &rhs) noexcept {
    return lhs.sign_ == rhs.sign_ && lhs.digits_ == rhs.digits_;
}

std::strong_ordering mini_llvm::operator<=>(const BigInteger &lhs, const BigInteger &rhs) noexcept {
    if (lhs.sign_ != rhs.sign_) {
        return lhs.sign_ <=> rhs.sign_;
    }
    std::strong_ordering result = cmpImpl(lhs.digits_, rhs.digits_);
    if (lhs.sign_ < 0) {
        result = 0 <=> result;
    }
    return result;
}

BigInteger mini_llvm::operator+(const BigInteger &lhs, const BigInteger &rhs) {
    if (lhs.sign_ == 0) {
        return rhs;
    }
    if (rhs.sign_ == 0) {
        return lhs;
    }
    if (lhs.sign_ != rhs.sign_) {
        std::strong_ordering result = cmpImpl(lhs.digits_, rhs.digits_);
        if (result == std::strong_ordering::equal) {
            return BigInteger();
        }
        if (result == std::strong_ordering::less) {
            return BigInteger(-lhs.sign_, subImpl(rhs.digits_, lhs.digits_));
        }
        return BigInteger(lhs.sign_, subImpl(lhs.digits_, rhs.digits_));
    }
    return BigInteger(lhs.sign_, addImpl(lhs.digits_, rhs.digits_));
}

BigInteger mini_llvm::operator-(const BigInteger &lhs, const BigInteger &rhs) {
    if (lhs.sign_ == 0) {
        return -rhs;
    }
    if (rhs.sign_ == 0) {
        return lhs;
    }
    if (lhs.sign_ != rhs.sign_) {
        return BigInteger(lhs.sign_, addImpl(lhs.digits_, rhs.digits_));
    }
    std::strong_ordering result = cmpImpl(lhs.digits_, rhs.digits_);
    if (result == std::strong_ordering::equal) {
        return BigInteger();
    }
    if (result == std::strong_ordering::less) {
        return BigInteger(-lhs.sign_, subImpl(rhs.digits_, lhs.digits_));
    }
    return BigInteger(lhs.sign_, subImpl(lhs.digits_, rhs.digits_));
}

BigInteger mini_llvm::operator*(const BigInteger &lhs, const BigInteger &rhs) {
    return BigInteger(lhs.sign_ * rhs.sign_, mulImpl(lhs.digits_, rhs.digits_));
}

BigInteger mini_llvm::operator/(const BigInteger &lhs, int32_t rhs) {
    assert(rhs != 0);
    std::vector<uint32_t> digits = divRemImpl(lhs.digits_, uabs(rhs)).first;
    int sign;
    if (digits.empty()) {
        sign = 0;
    } else if ((lhs.sign_ < 0) == (rhs < 0)) {
        sign = 1;
    } else {
        sign = -1;
    }
    return BigInteger(sign, std::move(digits));
}

int32_t mini_llvm::operator%(const BigInteger &lhs, int32_t rhs) {
    assert(rhs != 0);
    uint32_t r = divRemImpl(lhs.digits_, uabs(rhs)).second;
    if (lhs.sign_ < 0) {
        r = -r;
    }
    return static_cast<int32_t>(r);
}

std::pair<BigInteger, int32_t> mini_llvm::divRem(const BigInteger &lhs, int32_t rhs) {
    assert(rhs != 0);
    if (lhs.sign_ == 0) {
        return {BigInteger(), 0};
    }
    std::vector<uint32_t> digits;
    int32_t r;
    std::tie(digits, r) = divRemImpl(lhs.digits_, uabs(rhs));
    int sign = 1;
    if (digits.empty()) {
        sign = 0;
    } else if ((lhs.sign_ < 0) == (rhs < 0)) {
        sign = 1;
    } else {
        sign = -1;
    }
    if (lhs.sign_ < 0) {
        r = -r;
    }
    return {BigInteger(sign, std::move(digits)), r};
}

BigInteger mini_llvm::abs(const BigInteger &value) {
    return BigInteger(std::abs(value.sign_), value.digits_);
}

BigInteger mini_llvm::abs(BigInteger &&value) {
    return BigInteger(std::abs(value.sign_), std::move(value.digits_));
}
