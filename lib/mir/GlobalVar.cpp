#include "mini-llvm/mir/GlobalVar.h"

#include <algorithm>
#include <cctype>
#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm::mir;

std::string GlobalVar::format() const {
    if (isDeclaration()) {
        return std::format("undefined {:o}", *this);
    }
    StringJoiner formatted(" ");
    if (linkage() == Linkage::kInternal) {
        formatted.add("internal");
    }
    if (linkage() == Linkage::kPrivate) {
        formatted.add("private");
    }
    if (isConstant()) {
        formatted.add("constant");
    }
    formatted.add("{:o} = {}", *this, initializer());
    return formatted.toString();
}

std::string GlobalVar::formatAsOperand() const {
    if (!std::ranges::all_of(name(), [](char ch) { return isalnum(ch) || ch == '_' || ch == '.'; })) {
        return "@" + quote(name());
    }
    return "@" + name();
}
