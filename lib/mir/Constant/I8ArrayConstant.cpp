#include "mini-llvm/mir/Constant/I8ArrayConstant.h"

#include <cstdint>
#include <format>
#include <string>

#include "mini-llvm/utils/Chars.h"

using namespace mini_llvm::mir;

std::string I8ArrayConstant::format() const {
    std::string formatted;
    formatted += "\"";
    for (int8_t element : elements()) {
        if (element == '\\') {
            formatted += "\\\\";
        } else if (isPrintable(element) && element != '"') {
            formatted += element;
        } else {
            formatted += std::format("\\{:02X}", static_cast<uint8_t>(element));
        }
    }
    formatted += "\"";
    return formatted;
}
