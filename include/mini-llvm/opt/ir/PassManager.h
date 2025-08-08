// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT PassManager {
public:
    explicit PassManager(int pointerSize)
        : pointerSize_(pointerSize) {}

    void run(Module &M) const;

private:
    int pointerSize_;
};

} // namespace mini_llvm::ir
