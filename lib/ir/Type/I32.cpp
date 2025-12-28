// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/I32.h"

#include <cstdint>
#include <memory>

#include "mini-llvm/common/ops/Trunc.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I64.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::unique_ptr<Constant> I32::zeroValue() const {
    return std::make_unique<I32Constant>(0);
}

std::unique_ptr<Constant> I32::constant(int64_t value) const {
    return std::make_unique<I32Constant>(ops::Trunc<int32_t>()(value));
}

std::unique_ptr<Type> I32::promoted() const {
    return std::make_unique<I64>();
}

std::unique_ptr<Type> I32::demoted() const {
    return std::make_unique<I16>();
}
