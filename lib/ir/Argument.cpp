#include "mini-llvm/ir/Argument.h"

#include <algorithm>
#include <cstdint>
#include <format>
#include <string>

#include "mini-llvm/utils/Chars.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::string Argument::formatAsOperand() const {
    if (name().empty()) {
        return std::format("%_{}", toString(reinterpret_cast<uintptr_t>(this), 62));
    }
    if (!std::ranges::all_of(name(), [](char ch) { return isLetterOrDigit(ch) || ch == '_' || ch == '.'; })) {
        return std::format("%{}", quote(name()));
    }
    return std::format("%{}", name());
}
