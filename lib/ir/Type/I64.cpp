// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/I64.h"

#include <cstdint>
#include <memory>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I32.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> I64::zeroValue() const {
    return std::make_unique<I64Constant>(0);
}

std::unique_ptr<Constant> I64::constant(int64_t value) const {
    return std::make_unique<I64Constant>(value);
}

std::unique_ptr<Type> I64::demoted() const {
    return std::make_unique<I32>();
}
