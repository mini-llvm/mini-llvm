#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"

namespace mini_llvm {

class RISCVBackendDriver {
public:
    void run(const ir::Module &IM, mir::Module &MM, mc::Module &MCM);
};

} // namespace mini_llvm
