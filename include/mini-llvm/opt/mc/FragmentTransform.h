#pragma once

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/opt/mc/ProgramTransform.h"

namespace mini_llvm::mc {

class FragmentTransform : public ProgramTransform {
public:
    virtual bool runOnFragment(Fragment &fragment) = 0;
    bool runOnProgram(Program &program) override;
};

} // namespace mini_llvm::mc
