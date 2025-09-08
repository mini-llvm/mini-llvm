// SPDX-License-Identifier: MIT

#include "mini-llvm/utils/Dot.h"

#include <format>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Ascii.h"

bool mini_llvm::shouldQuoteDotId(std::string_view id) {
    if (id.empty()) {
        return true;
    }
    if (id == "digraph" || id == "edge" || id == "graph" || id == "node" || id == "strict" || id == "subgraph") {
        return true;
    }
    for (char ch : id) {
        if (!isAsciiLetterOrDigit(ch) && ch != '_') {
            return true;
        }
    }
    bool allDigits = true;
    for (char ch : id) {
        if (!isAsciiDigit(ch)) {
            allDigits = false;
            break;
        }
    }
    if (!allDigits && isAsciiDigit(id.front())) {
        return true;
    }
    return false;
}

std::string mini_llvm::quoteDotId(std::string_view id) {
    std::string quoted;
    quoted += '"';
    for (char ch : id) {
        switch (ch) {
        case '\n':
            quoted += "\\n";
            break;
        case '\r':
            quoted += "\\r";
            break;
        case '\\':
            quoted += "\\\\";
            break;
        case '\"':
            quoted += "\\\"";
            break;
        default:
            if (isAsciiPrintable(ch)) {
                quoted += ch;
            } else {
                quoted += std::format("\\{:03o}", static_cast<uint8_t>(ch));
            }
        }
    }
    quoted += '"';
    return quoted;
}

std::string mini_llvm::formatDotId(std::string_view id) {
    if (shouldQuoteDotId(id)) {
        return quoteDotId(id);
    }
    return std::string(id);
}
