// SPDX-License-Identifier: MIT

#pragma once

#include <memory>

#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT RISCVMCGen {
public:
    RISCVMCGen(const mir::Module *MM, mc::Module *MCM);

    ~RISCVMCGen();

    RISCVMCGen(RISCVMCGen &&) noexcept;
    RISCVMCGen &operator=(RISCVMCGen &&) noexcept;

    void emit();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm
