// SPDX-License-Identifier: MIT

#pragma once

#include <memory>

#include "mini-llvm/codegen/RegisterAllocator.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mc/Module.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm {

class MINI_LLVM_EXPORT RISCVBackendDriver {
public:
    explicit RISCVBackendDriver(RegisterAllocator *allocator);

    ~RISCVBackendDriver();

    RISCVBackendDriver(RISCVBackendDriver &&) noexcept;
    RISCVBackendDriver &operator=(RISCVBackendDriver &&) noexcept;

    void run(const ir::Module &IM, mir::Module &MM, mc::Module &MCM);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace mini_llvm
