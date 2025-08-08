// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Instruction/Load.h"

#include <string>

#include "mini-llvm/mir/FormatExtensionMode.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string Load::format() const {
    StringJoiner formatted(", ");
    formatted.add("LOAD i{} {}, {}", width() * 8, *dst(), src());
    if (extMode() != ExtensionMode::kNo) {
        formatted.add(specifier(extMode()));
    }
    return formatted.toString();
}
