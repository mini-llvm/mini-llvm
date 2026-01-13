// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/Store.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/BasicBlockType.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

bool Store::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*value() == this || &*ptr() == this) {
        return false;
    }
    std::unique_ptr<Type> valueType = value()->type();
    if (*valueType == Void() || *valueType == BasicBlockType()) {
        return false;
    }
    std::unique_ptr<Type> ptrType = ptr()->type();
    if (*ptrType != Ptr()) {
        return false;
    }
    return true;
}

std::string Store::format() const {
    return std::format("store {} {:o}, {} {:o}", *value()->type(), *value(), *ptr()->type(), *ptr());
}

std::unique_ptr<Value> Store::clone() const {
    return std::make_unique<Store>(share(*value()), share(*ptr()));
}
