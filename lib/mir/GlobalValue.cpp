#include "mini-llvm/mir/GlobalValue.h"

#include <algorithm>
#include <cctype>
#include <string>

#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm::mir;

std::string GlobalValue::formatAsOperand() const {
    if (!std::ranges::all_of(name(), [](char ch) { return isalnum(ch) || ch == '_' || ch == '.'; })) {
        return "@" + quote(name());
    }
    return "@" + name();
}
