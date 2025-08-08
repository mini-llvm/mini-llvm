// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Constant/PtrConstant.h"

#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string PtrConstant::format() const {
    StringJoiner formatted(" ");
    formatted.add("ptr({})", ptrSize());
    std::string formattedValue;
    auto [basePtr, offset] = value();
    if (basePtr == nullptr) {
        formattedValue = "null";
    } else {
        formattedValue = std::format("{:o}", *basePtr);
    }
    if (offset != 0) {
        formattedValue += std::format("{:+}", offset);
    }
    formatted.add("{}", formattedValue);
    return formatted.toString();
}
