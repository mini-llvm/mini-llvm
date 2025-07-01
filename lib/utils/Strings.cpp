#include "mini-llvm/utils/Strings.h"

#include <string>

void mini_llvm::normalizeLineEndings(std::string &str) {
    size_t n = str.size();
    size_t i = 0, j = 0;

    while (i < n) {
        if (str[i] == '\r') {
            ++i;
            if (i < n && str[i] == '\n') {
                ++i;
            }
            str[j++] = '\n';
        } else {
            str[j++] = str[i++];
        }
    }

    str.resize(j);
}
