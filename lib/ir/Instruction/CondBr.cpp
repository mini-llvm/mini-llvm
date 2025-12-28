// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/CondBr.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

bool CondBr::isWellFormed() const {
    if (!Terminator::isWellFormed()) {
        return false;
    }
    if (&*cond() == this) {
        return false;
    }
    if (*cond()->type() != I1()) {
        return false;
    }
    return true;
}

std::string CondBr::format() const {
    return std::format(
        "br {} {:o}, {} {:o}, {} {:o}",
        *cond()->type(), *cond(), *trueDest()->type(), *trueDest(), *falseDest()->type(), *falseDest());
}

std::unique_ptr<Value> CondBr::clone() const {
    return std::make_unique<CondBr>(share(*cond()), weaken(*trueDest()), weaken(*falseDest()));
}
