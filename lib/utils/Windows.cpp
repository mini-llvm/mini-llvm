#ifdef _WIN32

#include "mini-llvm/utils/Windows.h"

#include <string>
#include <vector>

#include <windows.h>

std::wstring mini_llvm::windows::widen(const char *str) {
    if (*str == '\0') {
        return {};
    }
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    std::wstring wstr(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr.data(), size);
    wstr.pop_back();
    return wstr;
}

std::string mini_llvm::windows::narrow(const wchar_t *wstr) {
    if (*wstr == L'\0') {
        return {};
    }
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
    std::string str(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str.data(), size, nullptr, nullptr);
    str.pop_back();
    return str;
}

std::vector<std::string> mini_llvm::windows::narrowArgs(int argc, wchar_t *wargv[]) {
    std::vector<std::string> args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i) {
        args.emplace_back(narrow(wargv[i]));
    }
    return args;
}

#endif // _WIN32
