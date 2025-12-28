// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/opt/mir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT TailDuplication final : public FunctionTransform {
public:
    explicit TailDuplication(size_t threshold) : threshold_(threshold) {}

    bool runOnFunction(Function &F) override;

private:
    size_t threshold_;
};

} // namespace mini_llvm::mir
