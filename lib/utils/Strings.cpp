#include "mini-llvm/utils/Strings.h"

#include <format>
#include <string>
#include <string_view>

#include "mini-llvm/utils/Chars.h"

using namespace mini_llvm;

std::string mini_llvm::quote(std::string_view str) {
    std::string quoted;
    quoted += '"';
    for (char ch : str) {
        if (ch == '\\') {
            quoted += "\\\\";
        } else if (isPrintable(ch) && ch != '"') {
            quoted += ch;
        } else {
            quoted += std::format("\\{:02X}", ch);
        }
    }
    quoted += '"';
    return quoted;
}
