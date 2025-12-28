// SPDX-License-Identifier: MIT

#ifdef _WIN32

#include "mini-llvm/utils/Windows.h"

#include <string>
#include <string_view>

#include <windows.h>

std::wstring mini_llvm::windows::widen(std::string_view str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
    std::wstring wstr(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), wstr.data(), size);
    return wstr;
}

std::string mini_llvm::windows::narrow(std::wstring_view wstr) {
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    std::string str(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), str.data(), size, nullptr, nullptr);
    return str;
}

#endif // _WIN32
