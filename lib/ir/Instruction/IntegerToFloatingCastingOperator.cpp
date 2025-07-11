#include "mini-llvm/ir/Instruction/IntegerToFloatingCastingOperator.h"

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/ir/Type/Ptr.h"

using namespace mini_llvm::ir;

bool IntegerToFloatingCastingOperator::isFoldable() const {
    return dynamic_cast<const Constant *>(&*value());
}

bool IntegerToFloatingCastingOperator::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*value() == this) {
        return false;
    }
    if (!dynamic_cast<const IntegerType *>(&*value()->type())) {
        return false;
    }
    if (*value()->type() == Ptr()) {
        return false;
    }
    return true;
}
