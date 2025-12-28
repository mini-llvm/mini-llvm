// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/mc/RISCVDataDirective.h"

#include <cstdlib>
#include <format>
#include <string>

using namespace mini_llvm::mc;

std::string RISCVDataDirective::format() const {
    const char *directiveName;
    switch (width()) {
        case 1: directiveName = "byte"; break;
        case 2: directiveName = "half"; break;
        case 4: directiveName = "word"; break;
        case 8: directiveName = "dword"; break;
        default: abort();
    }
    return std::format(".{} {}", directiveName, value());
}
