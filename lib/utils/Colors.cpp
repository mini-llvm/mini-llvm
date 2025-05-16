#include "mini-llvm/utils/Colors.h"

#include <cstdlib>
#include <cstring>

#ifdef _WIN32
    #include <io.h>
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using namespace mini_llvm;

bool mini_llvm::supportsColor(FILE *stream) {
#ifdef _WIN32
    int fd = _fileno(stream);
    if (fd == -1) {
        return false;
    }
    if (!_isatty(fd)) {
        return false;
    }
    HANDLE hConsole = (HANDLE)_get_osfhandle(fd);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD dwMode;
    if (!GetConsoleMode(hConsole, &dwMode)) {
        return false;
    }
    if (!(dwMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hConsole, dwMode)) {
            return false;
        }
    }
    return true;
#else
    int fd = fileno(stream);
    if (fd == -1) {
        return false;
    }
    if (!isatty(fd)) {
        return false;
    }
    const char *term = getenv("TERM");
    if (term == nullptr) {
        return false;
    }
    if (strcmp(term, "dumb") == 0) {
        return false;
    }
    return true;
#endif
}
