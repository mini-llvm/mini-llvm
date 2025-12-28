// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/PtrToInt.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

bool PtrToInt::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*value() == this) {
        return false;
    }
    if (*value()->type() != Ptr()) {
        return false;
    }
    return true;
}

std::string PtrToInt::format() const {
    return std::format("{:o} = ptrtoint {} {:o} to {}", *this, *value()->type(), *value(), *type());
}

std::unique_ptr<Value> PtrToInt::clone() const {
    return std::make_unique<PtrToInt>(share(*value()), cast<IntegerType>(type()));
}
