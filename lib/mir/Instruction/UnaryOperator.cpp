// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Instruction/UnaryOperator.h"

#include <string>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/mir/FormatExtensionMode.h"
#include "mini-llvm/utils/StringJoiner.h"

namespace mini_llvm::mir {

std::string UnaryOperator::format() const {
    StringJoiner formatted(", ");
    formatted.add("{} i{} {}, {}", mnemonic(), width() * 8, *dst(), *src());
    if (extMode() != ExtensionMode::kNo) {
        formatted.add(specifier(extMode()));
    }
    return formatted.toString();
}

} // namespace mini_llvm::mir
