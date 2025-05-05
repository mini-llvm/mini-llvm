#include "mini-llvm/utils/Strings.h"

#include <format>
#include <string>
#include <string_view>

using namespace mini_llvm;

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
