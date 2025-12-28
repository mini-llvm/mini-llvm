// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/IntegerOrPointerType.h"

using namespace mini_llvm::ir;

bool BinaryIntegerOperator::isFoldable() const {
    return dynamic_cast<const Constant *>(&*lhs()) && dynamic_cast<const Constant *>(&*rhs());
}

bool BinaryIntegerOperator::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*lhs() == this || &*rhs() == this) {
        return false;
    }
    if (*lhs()->type() != *rhs()->type()) {
        return false;
    }
    if (!dynamic_cast<const IntegerOrPointerType *>(&*lhs()->type())) {
        return false;
    }
    return true;
}
