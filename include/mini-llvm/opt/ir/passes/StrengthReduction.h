// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/opt/ir/FunctionTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT StrengthReduction final : public FunctionTransform {
public:
    StrengthReduction(size_t mulThreshold, size_t divThreshold, size_t remThreshold)
        : mulThreshold_(mulThreshold), divThreshold_(divThreshold), remThreshold_(remThreshold) {}

    bool runOnFunction(Function &F) override;

private:
    size_t mulThreshold_;
    size_t divThreshold_;
    size_t remThreshold_;
};

} // namespace mini_llvm::ir
