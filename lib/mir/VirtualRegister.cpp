// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/VirtualRegister.h"

#include <cstdint>
#include <string>

#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm::mir;

std::string VirtualRegister::format() const {
    if (name().empty()) {
        return "%_" + toString(reinterpret_cast<uintptr_t>(this), 62);
    }
    return "%" + name();
}
