// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/Float.h"

#include <bit>
#include <cstdint>
#include <memory>

#include "mini-llvm/common/ops/Trunc.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Double.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::unique_ptr<Constant> Float::zeroValue() const {
    return std::make_unique<FloatConstant>(0.f);
}

std::unique_ptr<Constant> Float::constant(int64_t value) const {
    return std::make_unique<FloatConstant>(std::bit_cast<float>(ops::Trunc<int32_t>()(value)));
}

std::unique_ptr<Type> Float::promoted() const {
    return std::make_unique<Double>();
}
