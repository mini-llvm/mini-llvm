#pragma once

#include <optional>
#include <vector>

#include "mini-llvm/common/Diagnostic.h"
#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

std::optional<Module> parseModule(const char *source);
std::optional<Module> parseModule(const char *source, std::vector<Diagnostic> &diags);

} // namespace mini_llvm::ir
