// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Constant/I32ArrayConstant.h"

#include <cstdint>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string I32ArrayConstant::format() const {
    StringJoiner formatted("\n", "i32 [\n", "\n]");
    for (int32_t element : elements()) {
        formatted.add("  {}", element);
    }
    return formatted.toString();
}
