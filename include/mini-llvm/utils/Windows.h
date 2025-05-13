#pragma once

#ifdef _WIN32

#include <string>
#include <vector>

namespace mini_llvm::windows {

// https://utf8everywhere.org/#windows

std::wstring widen(const char *str);

inline std::wstring widen(const std::string &str) {
    return widen(str.c_str());
}

std::string narrow(const wchar_t *wstr);

inline std::string narrow(const std::wstring &wstr) {
    return narrow(wstr.c_str());
}

std::vector<std::string> narrowArgs(int argc, wchar_t *wargv[]);

} // namespace mini_llvm::windows

#endif // _WIN32
