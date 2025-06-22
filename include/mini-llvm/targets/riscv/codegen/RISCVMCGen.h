#pragma once

#include <memory>

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"

namespace mini_llvm {

class RISCVMCGen {
public:
    RISCVMCGen(const mir::Module *MM, mc::Module *MCM);
    ~RISCVMCGen();

    void emit();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm
