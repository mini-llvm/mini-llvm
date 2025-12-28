// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/Ptr.h"

#include <memory>

#include "mini-llvm/ir/Constant/NullPtrConstant.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> Ptr::zeroValue() const {
    return std::make_unique<NullPtrConstant>();
}
