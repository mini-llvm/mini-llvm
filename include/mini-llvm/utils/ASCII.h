#pragma once

namespace mini_llvm {

bool isPrintable(char ch) noexcept;
bool isUpper(char ch) noexcept;
bool isLower(char ch) noexcept;
bool isLetter(char ch) noexcept;
bool isDigit(char ch) noexcept;
bool isLetterOrDigit(char ch) noexcept;
bool isHexDigit(char ch) noexcept;

} // namespace mini_llvm
