// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT Pass {
public:
    virtual ~Pass() = default;
    Pass() = default;
    Pass(const Pass &) = delete;
    Pass(Pass &&) = delete;
    Pass &operator=(const Pass &) = delete;
    Pass &operator=(Pass &&) = delete;

    virtual const char *name() const = 0;
};

} // namespace mini_llvm::mir
