#pragma once

#ifdef _WIN32

#include <string>
#include <string_view>
#include <vector>

namespace mini_llvm::windows {

// https://utf8everywhere.org/#windows

std::wstring widen(std::string_view str);
std::string narrow(std::wstring_view wstr);

} // namespace mini_llvm::windows

#endif // _WIN32
