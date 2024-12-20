#pragma once

#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

Module parseModule(const char *source);

} // namespace mini_llvm::ir
