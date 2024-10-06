#pragma once

#include "mini-llvm/mc/Program.h"

namespace mini_llvm::mc {

class RISCVPassManager {
public:
    void run(Program &program) const;
};

} // namespace mini_llvm::mc
