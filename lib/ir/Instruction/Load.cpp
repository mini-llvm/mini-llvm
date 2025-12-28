// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/Load.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

bool Load::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*ptr() == this) {
        return false;
    }
    if (*ptr()->type() != Ptr()) {
        return false;
    }
    return true;
}

std::string Load::format() const {
    return std::format("{:o} = load {}, {} {:o}", *this, *type(), *ptr()->type(), *ptr());
}

std::unique_ptr<Value> Load::clone() const {
    return std::make_unique<Load>(type(), share(*ptr()));
}
