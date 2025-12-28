// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/I8.h"

#include <cstdint>
#include <memory>

#include "mini-llvm/common/ops/Trunc.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I16.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::unique_ptr<Constant> I8::zeroValue() const {
    return std::make_unique<I8Constant>(static_cast<int8_t>(0));
}

std::unique_ptr<Constant> I8::constant(int64_t value) const {
    return std::make_unique<I8Constant>(ops::Trunc<int8_t>()(value));
}

std::unique_ptr<Type> I8::promoted() const {
    return std::make_unique<I16>();
}
