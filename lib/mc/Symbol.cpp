// SPDX-License-Identifier: MIT

#include "mini-llvm/mc/Symbol.h"

#include <cstddef>
#include <string>

#include "mini-llvm/utils/Ascii.h"

using namespace mini_llvm;
using namespace mini_llvm::mc;

namespace {

bool shouldQuote(const std::string &name) {
    size_t n = name.size();
    if (n == 0) {
        return true;
    }
    if (!isAsciiLetter(name[0]) && name[0] != '.' && name[0] != '_') {
        return true;
    }
    for (size_t i = 1; i < n; ++i) {
        if (!isAsciiLetterOrDigit(name[i]) && name[i] != '.' && name[i] != '_') {
            return true;
        }
    }
    return false;
}

std::string quote(const std::string &name) {
    std::string quoted;
    quoted += '"';
    for (char ch : name) {
        switch (ch) {
        case '\n':
            quoted += "\\n";
            break;
        case '\\':
            quoted += "\\\\";
            break;
        case '\"':
            quoted += "\\\"";
            break;
        default:
            quoted += ch;
        }
    }
    quoted += '"';
    return quoted;
}

} // namespace

std::string Symbol::format() const {
    if (shouldQuote(name())) {
        return quote(name());
    }
    return name();
}
