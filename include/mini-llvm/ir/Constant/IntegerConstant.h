// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT IntegerConstant : public Constant {
public:
    virtual int64_t signExtendedValue() const = 0;
    virtual int64_t zeroExtendedValue() const = 0;
};

} // namespace mini_llvm::ir
