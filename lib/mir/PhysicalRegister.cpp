#include "mini-llvm/mir/PhysicalRegister.h"

#include <algorithm>
#include <cctype>
#include <string>

#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

std::string PhysicalRegister::format() const {
    if (!std::ranges::all_of(name(), [](char ch) { return isalnum(ch) || ch == '_' || ch == '.'; })) {
        return "$" + quote(name());
    }
    return "$" + name();
}
