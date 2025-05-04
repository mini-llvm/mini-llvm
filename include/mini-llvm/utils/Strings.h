#pragma once

#include <algorithm>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

namespace mini_llvm {

void computeLineColumn(const char *start, const char *location, size_t &line, size_t &column);

std::string quote(std::string_view str);

template <typename T>
    requires std::is_integral_v<T>
std::string toString(T value, int base = 10) {
    if (value < 0) {
        return "-" + toString(-value, base);
    }

    std::string result;

    do {
        result.push_back("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[value % base]);
        value /= base;
    } while (value > 0);

    std::reverse(result.begin(), result.end());

    return result;
}

} // namespace mini_llvm
