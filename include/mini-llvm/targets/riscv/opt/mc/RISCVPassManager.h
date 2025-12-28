// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT RISCVPassManager {
public:
    void run(Module &M) const;
};

} // namespace mini_llvm::mc
