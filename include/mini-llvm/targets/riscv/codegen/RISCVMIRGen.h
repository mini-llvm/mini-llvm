// SPDX-License-Identifier: MIT

#pragma once

#include <memory>

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT RISCVMIRGen {
public:
    // NOLINTNEXTLINE(cppcoreguidelines-use-enum-class)
    enum : int { kSave, kRestore };

    RISCVMIRGen(const ir::Module *IM, mir::Module *MM);

    ~RISCVMIRGen();

    RISCVMIRGen(const RISCVMIRGen &) = delete;
    RISCVMIRGen &operator=(const RISCVMIRGen &) = delete;

    RISCVMIRGen(RISCVMIRGen &&) noexcept;
    RISCVMIRGen &operator=(RISCVMIRGen &&) noexcept;

    void emit();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm
