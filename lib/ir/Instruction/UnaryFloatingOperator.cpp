// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/UnaryFloatingOperator.h"

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/FloatingType.h"

using namespace mini_llvm::ir;

bool UnaryFloatingOperator::isFoldable() const {
    return dynamic_cast<const Constant *>(&*value());
}

bool UnaryFloatingOperator::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*value() == this) {
        return false;
    }
    if (!dynamic_cast<const FloatingType *>(&*value()->type())) {
        return false;
    }
    return true;
}
