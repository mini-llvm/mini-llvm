// SPDX-License-Identifier: MIT

#include "mini-llvm/mc/StringDirective.h"

#include <cstdint>
#include <format>
#include <string>

#include "mini-llvm/utils/Ascii.h"

using namespace mini_llvm::mc;

std::string StringDirective::format() const {
    std::string formatted;
    formatted += ".ascii \"";
    for (int8_t element : elements_) {
        switch (element) {
        case '\b':
            formatted += "\\b";
            break;
        case '\f':
            formatted += "\\f";
            break;
        case '\n':
            formatted += "\\n";
            break;
        case '\r':
            formatted += "\\r";
            break;
        case '\t':
            formatted += "\\t";
            break;
        case '\\':
            formatted += "\\\\";
            break;
        case '\"':
            formatted += "\\\"";
            break;
        default:
            if (isAsciiPrintable(element)) {
                formatted += element;
            } else {
                formatted += std::format("\\{:03o}", static_cast<uint8_t>(element));
            }
        }
    }
    formatted += "\"";
    return formatted;
}
