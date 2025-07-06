#include "mini-llvm/utils/ASCII.h"

bool mini_llvm::isPrintable(char ch) noexcept {
    return ch >= 0x20 && ch <= 0x7e;
}

bool mini_llvm::isUpper(char ch) noexcept {
    return 'A' <= ch && ch <= 'Z';
}

bool mini_llvm::isLower(char ch) noexcept {
    return 'a' <= ch && ch <= 'z';
}

bool mini_llvm::isLetter(char ch) noexcept {
    return isUpper(ch) || isLower(ch);
}

bool mini_llvm::isDigit(char ch) noexcept {
    return '0' <= ch && ch <= '9';
}

bool mini_llvm::isLetterOrDigit(char ch) noexcept {
    return isLetter(ch) || isDigit(ch);
}

bool mini_llvm::isHexDigit(char ch) noexcept {
    return ('0' <= ch && ch <= '9') || ('A' <= ch && ch <= 'F') || ('a' <= ch && ch <= 'f');
}
