// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/IntToPtr.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/IntegerType.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

bool IntToPtr::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*value() == this) {
        return false;
    }
    if (!dynamic_cast<const IntegerType *>(&*value()->type())) {
        return false;
    }
    return true;
}

std::string IntToPtr::format() const {
    return std::format("{:o} = inttoptr {} {:o} to {}", *this, *value()->type(), *value(), *type());
}

std::unique_ptr<Value> IntToPtr::clone() const {
    return std::make_unique<IntToPtr>(share(*value()));
}
