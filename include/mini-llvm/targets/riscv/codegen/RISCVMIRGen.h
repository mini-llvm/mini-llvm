#pragma once

#include <memory>

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mir/Module.h"

namespace mini_llvm {

class RISCVMIRGen {
public:
    enum { kSave, kRestore };

    RISCVMIRGen(const ir::Module *IM, mir::Module *MM);
    ~RISCVMIRGen();

    void emit();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm
