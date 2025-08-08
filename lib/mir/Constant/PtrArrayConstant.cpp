// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Constant/PtrArrayConstant.h"

#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string PtrArrayConstant::format() const {
    StringJoiner formattedElements("\n", "[\n", "\n]");
    for (auto [basePtr, offset] : elements()) {
        std::string formattedElement;
        if (basePtr == nullptr) {
            formattedElement = "null";
        } else {
            formattedElement = std::format("{:o}", *basePtr);
        }
        if (offset != 0) {
            formattedElement += std::format("{:+}", offset);
        }
        formattedElements.add("  {}", formattedElement);
    }
    return std::format("ptr({}) {}", ptrSize(), formattedElements);
}
