// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/opt/mir/BasicBlockTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT NullOperationElimination final : public BasicBlockTransform {
public:
    bool runOnBasicBlock(BasicBlock &B) override;
};

} // namespace mini_llvm::mir
