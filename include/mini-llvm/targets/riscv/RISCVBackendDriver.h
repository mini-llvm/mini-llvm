#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/mir/Module.h"

namespace mini_llvm {

class RISCVBackendDriver {
public:
    void run(const ir::Module &IM, mir::Module &MM, mc::Program &program);
};

} // namespace mini_llvm
