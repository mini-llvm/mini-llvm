// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/GlobalValue.h"

#include <cstdint>
#include <string>

#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm::mir;

std::string GlobalValue::formatAsOperand() const {
    if (name().empty()) {
        return "@_" + toString(reinterpret_cast<uintptr_t>(this), 62);
    }
    return "@" + name();
}
