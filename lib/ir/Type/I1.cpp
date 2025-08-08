// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/I1.h"

#include <cstdint>
#include <memory>

#include "mini-llvm/common/ops/Trunc.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

std::unique_ptr<Constant> I1::zeroValue() const {
    return std::make_unique<I1Constant>(false);
}

std::unique_ptr<Constant> I1::constant(int64_t value) const {
    return std::make_unique<I1Constant>(ops::Trunc<bool>()(value));
}
