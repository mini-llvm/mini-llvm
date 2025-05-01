#pragma once

#include "mini-llvm/mc/Program.h"
#include "mini-llvm/opt/mc/Pass.h"

namespace mini_llvm::mc {

class ProgramTransform : public Pass {
public:
    virtual bool runOnProgram(Program &program) = 0;
};

} // namespace mini_llvm::mc
