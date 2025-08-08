// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/Select.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

bool Select::isFoldable() const {
    return dynamic_cast<const Constant *>(&*cond())
        && dynamic_cast<const Constant *>(&*trueValue())
        && dynamic_cast<const Constant *>(&*falseValue());
}

std::shared_ptr<Constant> Select::fold() const {
    if (dynamic_cast<const PoisonValue *>(&*cond())) {
        return std::make_shared<PoisonValue>(type());
    }
    if (static_cast<const I1Constant *>(&*cond())->value() == true) {
        return cast<Constant>(share(*trueValue()));
    } else {
        return cast<Constant>(share(*falseValue()));
    }
}

bool Select::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    if (&*cond() == this || &*trueValue() == this || &*falseValue() == this) {
        return false;
    }
    if (*cond()->type() != I1()) {
        return false;
    }
    if (*trueValue()->type() != *falseValue()->type()) {
        return false;
    }
    return true;
}

std::string Select::format() const {
    return std::format(
        "{:o} = select {} {:o}, {} {:o}, {} {:o}",
        *this, *cond()->type(), *cond(), *trueValue()->type(), *trueValue(), *falseValue()->type(), *falseValue());
}

std::unique_ptr<Value> Select::clone() const {
    return std::make_unique<Select>(share(*cond()), share(*trueValue()), share(*falseValue()));
}
