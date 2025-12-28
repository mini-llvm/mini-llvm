// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/I16.h"

#include <cstdint>
#include <memory>

#include "mini-llvm/common/ops/Trunc.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I8.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::unique_ptr<Constant> I16::zeroValue() const {
    return std::make_unique<I16Constant>(static_cast<int16_t>(0));
}

std::unique_ptr<Constant> I16::constant(int64_t value) const {
    return std::make_unique<I16Constant>(ops::Trunc<int16_t>()(value));
}

std::unique_ptr<Type> I16::promoted() const {
    return std::make_unique<I32>();
}

std::unique_ptr<Type> I16::demoted() const {
    return std::make_unique<I8>();
}
