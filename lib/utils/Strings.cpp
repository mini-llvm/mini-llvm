#include "mini-llvm/utils/Strings.h"

#include <format>
#include <string>
#include <string_view>

using namespace mini_llvm;

void mini_llvm::computeLineColumn(const char *start, const char *location, size_t &line, size_t &column) {
    line = column = 1;
    while (start != location) {
        if (*start == '\n') {
            ++line;
            column = 1;
        } else {
            ++column;
        }
        ++start;
    }
}

std::string mini_llvm::quote(std::string_view str) {
    std::string quoted;
    quoted += '"';
    for (char ch : str) {
        if (0x20 <= ch && ch <= 0x7e) {
            quoted += ch;
        } else {
            quoted += std::format("\\{:02X}", ch);
        }
    }
    quoted += '"';
    return quoted;
}
