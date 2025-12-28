// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT FloatingType : public Type {
public:
    virtual Precision precision() const = 0;
};

} // namespace mini_llvm::ir
