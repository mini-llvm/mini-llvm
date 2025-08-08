// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/Alloca.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/BasicBlockType.h"
#include "mini-llvm/ir/Type/Void.h"

using namespace mini_llvm::ir;

bool Alloca::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (*allocatedType() == Void() || *allocatedType() == BasicBlockType()) {
        return false;
    }
    return true;
}

std::string Alloca::format() const {
    return std::format("{:o} = alloca {}", *this, *allocatedType());
}

std::unique_ptr<Value> Alloca::clone() const {
    return std::make_unique<Alloca>(allocatedType());
}
