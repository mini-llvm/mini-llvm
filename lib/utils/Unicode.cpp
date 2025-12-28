// SPDX-License-Identifier: MIT

#include "mini-llvm/utils/Unicode.h"

char32_t mini_llvm::decodeUtf8(const char *&ptr) noexcept {
    char8_t first = static_cast<char8_t>(*ptr);
    char32_t cp = 0;
    int n = 0;

    if (first < 0x80) {
        cp = first;
        n = 1;
    } else if ((first & 0xe0) == 0xc0) {
        cp = first & 0x1f;
        n = 2;
    } else if ((first & 0xf0) == 0xe0) {
        cp = first & 0x0f;
        n = 3;
    } else if ((first & 0xf8) == 0xf0) {
        cp = first & 0x07;
        n = 4;
    } else {
        return static_cast<char32_t>(-1);
    }

    for (int i = 1; i < n; ++i) {
        char8_t b = static_cast<char8_t>(*(ptr + i));
        if ((b & 0xc0) != 0x80) {
            return static_cast<char32_t>(-1);
        }
        cp = (cp << 6) | (b & 0x3f);
    }

    if ((n == 2 && cp < 0x80) || (n == 3 && cp < 0x800) || (n == 4 && cp < 0x10000)) {
        return static_cast<char32_t>(-1);
    }
    if ((cp >= 0xd800 && cp <= 0xdfff) || cp > 0x10ffff) {
        return static_cast<char32_t>(-1);
    }

    ptr += n;
    return cp;
}
