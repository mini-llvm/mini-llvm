#include "mini-llvm/mir/PhysicalRegister.h"

#include <algorithm>
#include <string>

#include "mini-llvm/utils/Chars.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

std::string PhysicalRegister::format() const {
    if (!std::ranges::all_of(name(), [](char ch) { return isLetterOrDigit(ch) || ch == '_' || ch == '.'; })) {
        return "$" + quote(name());
    }
    return "$" + name();
}
