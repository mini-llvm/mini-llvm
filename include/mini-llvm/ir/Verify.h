#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

bool verifyFunction(const Function &F);
bool verifyModule(const Module &M);

} // namespace mini_llvm::ir
