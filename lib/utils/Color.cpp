// SPDX-License-Identifier: MIT

#include "mini-llvm/utils/Color.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

#ifdef _WIN32
    #include <io.h>
    #include <windows.h>
#else
    #include <unistd.h>
#endif

namespace mini_llvm {

bool supportsColor(FILE *stream) {
    // force-color.org
    if (const char *forceColor = getenv("FORCE_COLOR"); forceColor && *forceColor != '\0') {
        return true;
    }
    // no-color.org
    if (const char *noColor = getenv("NO_COLOR"); noColor && *noColor != '\0') {
        return false;
    }
#ifdef _WIN32
    int fd = _fileno(stream);
    if (fd < 0 || !_isatty(fd)) {
        return false;
    }
    HANDLE hConsole = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
    if (hConsole == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole, &dwMode)) {
        return false;
    }
    return (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
#else
    int fd = fileno(stream);
    if (fd < 0 || !isatty(fd)) {
        return false;
    }
    const char *term = getenv("TERM");
    if (!term) {
        return false;
    }
    if (strcmp(term, "dumb") == 0) {
        return false;
    }
    return true;
#endif
}

#ifdef _WIN32

VTModeGuard::VTModeGuard(FILE *stream, bool enableVTMode) : hConsole_(INVALID_HANDLE_VALUE), oldVTModeEnabled_(false) {
    int fd = _fileno(stream);
    if (fd < 0) {
        return;
    }
    HANDLE hConsole = reinterpret_cast<HANDLE>(_get_osfhandle(fd));
    if (hConsole == INVALID_HANDLE_VALUE) {
        return;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole, &dwMode)) {
        return;
    }
    hConsole_ = hConsole;
    oldVTModeEnabled_ = (dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
    DWORD dwNewMode = enableVTMode
        ? (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)
        : (dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (dwNewMode != dwMode) {
        SetConsoleMode(hConsole_, dwNewMode);
    }
}

VTModeGuard::~VTModeGuard() {
    if (hConsole_ == INVALID_HANDLE_VALUE) {
        return;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hConsole_, &dwMode)) {
        return;
    }
    DWORD dwOldMode = oldVTModeEnabled_
        ? (dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)
        : (dwMode & ~ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    if (dwOldMode != dwMode) {
        SetConsoleMode(hConsole_, dwOldMode);
    }
}

#endif // _WIN32

namespace {

thread_local bool g_colorEnabled = false;

} // namespace

ColorGuard::ColorGuard(bool enableColor) {
    oldColorEnabled_ = g_colorEnabled;
    g_colorEnabled = enableColor;
}

ColorGuard::~ColorGuard() {
    g_colorEnabled = oldColorEnabled_;
}

ColoredStringView::operator std::string() const {
    if (!g_colorEnabled) {
        return std::string(str);
    }
    if (codes.none()) {
        return std::string(str);
    }
    StringJoiner joiner(";", "\033[", "m");
#define X(code, name) \
    if (codes.test(code)) { \
        joiner.add("{}", code); \
    }
#include "mini-llvm/utils/Color.def"
#undef X
    return std::format("{}{}\033[0m", joiner, str);
}

} // namespace mini_llvm
