// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/Void.h"

#include <memory>

#include "mini-llvm/ir/Constant/VoidValue.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> Void::zeroValue() const {
    return std::make_unique<VoidValue>();
}
