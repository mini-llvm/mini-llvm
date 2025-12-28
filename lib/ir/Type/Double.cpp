// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/Double.h"

#include <bit>
#include <cstdint>
#include <memory>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Float.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> Double::zeroValue() const {
    return std::make_unique<DoubleConstant>(0.);
}

std::unique_ptr<Constant> Double::constant(int64_t value) const {
    return std::make_unique<DoubleConstant>(std::bit_cast<double>(value));
}

std::unique_ptr<Type> Double::demoted() const {
    return std::make_unique<Float>();
}
