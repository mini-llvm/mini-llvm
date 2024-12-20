#include "mini-llvm/utils/Strings.h"

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
