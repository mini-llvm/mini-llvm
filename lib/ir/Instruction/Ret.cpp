// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/Ret.h"

#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/Constant/VoidValue.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

std::string Ret::format() const {
    if (dynamic_cast<const VoidValue *>(&*value())) {
        return std::format("ret {}", *value()->type());
    }
    return std::format("ret {} {:o}", *value()->type(), *value());
}

std::unique_ptr<Value> Ret::clone() const {
    return std::make_unique<Ret>(share(*value()));
}
