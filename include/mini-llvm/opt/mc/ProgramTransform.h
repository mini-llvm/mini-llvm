#pragma once

#include "mini-llvm/mc/Program.h"

namespace mini_llvm::mc {

class ProgramTransform {
public:
    ~ProgramTransform() = default;
    virtual bool runOnProgram(Program &program) = 0;
};

} // namespace mini_llvm::mc
