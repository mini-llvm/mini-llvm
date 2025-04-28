#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

bool verify(const Function &F);
bool verify(const Module &M);

} // namespace mini_llvm::ir
