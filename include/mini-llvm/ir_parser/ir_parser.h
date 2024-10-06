#pragma once

#include <memory>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

Module parseModule(const char *source);
std::shared_ptr<GlobalVar> parseGlobalVar(const char *source);
std::shared_ptr<Function> parseFunction(const char *source);

} // namespace mini_llvm::ir
