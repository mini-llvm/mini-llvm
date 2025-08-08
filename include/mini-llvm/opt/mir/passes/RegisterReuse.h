// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/opt/mir/BasicBlockTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT RegisterReuse final : public BasicBlockTransform {
public:
    bool runOnBasicBlock(BasicBlock &B) override;

    const char *name() const override {
        return "RegisterReuse";
    }
};

} // namespace mini_llvm::mir
