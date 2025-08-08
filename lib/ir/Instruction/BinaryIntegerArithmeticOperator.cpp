// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"

#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Type/IntegerType.h"

using namespace mini_llvm::ir;

bool BinaryIntegerArithmeticOperator::isWellFormed() const {
    if (!BinaryIntegerOperator::isWellFormed()) {
        return false;
    }
    if (!dynamic_cast<const IntegerType *>(&*lhs()->type())) {
        return false;
    }
    return true;
}
