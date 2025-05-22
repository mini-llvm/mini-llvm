#pragma once

#include "mini-llvm/ir/Module.h"

namespace mini_llvm::ir {

class PassManager {
public:
    explicit PassManager(int pointerSize)
        : pointerSize_(pointerSize) {}

    void run(Module &M) const;

private:
    int pointerSize_;
};

} // namespace mini_llvm::ir
