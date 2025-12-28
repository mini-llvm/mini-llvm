// SPDX-License-Identifier: MIT

#include "mini-llvm/utils/Strings.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <type_traits>

namespace {

template <typename T>
std::string toStringImpl(T value, int base) {
    if (value < 0) {
        return "-" + toStringImpl(-static_cast<std::make_unsigned_t<T>>(value), base);
    }

    std::string result;

    do {
        result.push_back("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"[value % base]);
        value /= base;
    } while (value > 0);

    std::reverse(result.begin(), result.end());

    return result;
}

} // namespace

std::string mini_llvm::toString(int32_t value, int base) {
    return toStringImpl(value, base);
}

std::string mini_llvm::toString(int64_t value, int base) {
    return toStringImpl(value, base);
}

std::string mini_llvm::toString(uint32_t value, int base) {
    return toStringImpl(value, base);
}

std::string mini_llvm::toString(uint64_t value, int base) {
    return toStringImpl(value, base);
}

void mini_llvm::normalizeLineEndings(std::string &str) {
    size_t n = str.size();
    size_t i = 0, j = 0;

    while (i < n) {
        if (str[i] == '\r') {
            ++i;
            if (i < n && str[i] == '\n') {
                ++i;
            }
            str[j++] = '\n';
        } else {
            str[j++] = str[i++];
        }
    }

    str.resize(j);
}
