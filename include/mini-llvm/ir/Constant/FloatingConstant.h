// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FloatingConstant : public Constant {
public:
    virtual uint64_t bitPattern() const = 0;
};

} // namespace mini_llvm::ir
