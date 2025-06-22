#pragma once

#include "mini-llvm/mc/Module.h"

namespace mini_llvm::mc {

class RISCVPassManager {
public:
    void run(Module &M) const;
};

} // namespace mini_llvm::mc
