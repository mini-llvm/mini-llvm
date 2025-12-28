// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/opt/mc/GlobalValueTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT RISCVFallthrough final : public GlobalValueTransform {
public:
    bool runOnGlobalValue(GlobalValue &G) override;
};

} // namespace mini_llvm::mc
