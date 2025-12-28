// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/GlobalVar.h"

#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string GlobalVar::format() const {
    if (isDeclaration()) {
        return std::format("undefined {:o}", *this);
    }
    StringJoiner formatted(" ");
    switch (linkage()) {
    case Linkage::kInternal:
        formatted.add("internal");
        break;
    case Linkage::kPrivate:
        formatted.add("private");
        break;
    case Linkage::kExternal:
        break;
    }
    if (isConstant()) {
        formatted.add("constant");
    }
    formatted.add("align({})", alignment());
    formatted.add("{:o} = {}", *this, initializer());
    return formatted.toString();
}
