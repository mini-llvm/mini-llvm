// SPDX-License-Identifier: MIT

#pragma once

#include <optional>
#include <string>
#include <vector>

#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

MINI_LLVM_EXPORT std::optional<Module> parseModule(const char *source);

inline std::optional<Module> parseModule(const std::string &source) {
    return parseModule(source.c_str());
}

MINI_LLVM_EXPORT std::optional<Module> parseModule(const char *source, std::vector<Diagnostic> &diags);

inline std::optional<Module> parseModule(const std::string &source, std::vector<Diagnostic> &diags) {
    return parseModule(source.c_str(), diags);
}

} // namespace mini_llvm::ir
