#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"

#include "mini-llvm/ir/Instruction/BinaryIntegerOperator.h"
#include "mini-llvm/ir/Type/Ptr.h"

using namespace mini_llvm::ir;

bool BinaryIntegerArithmeticOperator::isWellFormed() const {
    if (!BinaryIntegerOperator::isWellFormed()) {
        return false;
    }
    if (*lhs()->type() == Ptr()) {
        return false;
    }
    return true;
}
