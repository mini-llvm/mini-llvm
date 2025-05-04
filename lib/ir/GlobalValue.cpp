#include "mini-llvm/ir/GlobalValue.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <format>
#include <string>

#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::string GlobalValue::formatAsOperand() const {
    if (name().empty()) {
        return std::format("@_{}", toString(reinterpret_cast<uintptr_t>(this), 62));
    }
    if (!std::ranges::all_of(name(), [](char ch) { return isalnum(ch) || ch == '_' || ch == '.'; })) {
        return std::format("@{}", quote(name()));
    }
    return std::format("@{}", name());
}
