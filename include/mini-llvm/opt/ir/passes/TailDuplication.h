// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT TailDuplication final : public FunctionTransform {
public:
    explicit TailDuplication(size_t threshold) : threshold_(threshold) {}

    bool runOnFunction(Function &F) override;

private:
    size_t threshold_;
};

} // namespace mini_llvm::ir
