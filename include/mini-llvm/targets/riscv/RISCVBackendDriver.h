#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mc/Program.h"

namespace mini_llvm {

class RISCVBackendDriver {
public:
    mc::Program run(const ir::Module &IM);
};

} // namespace mini_llvm
