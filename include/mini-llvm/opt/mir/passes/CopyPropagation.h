#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/opt/mir/BasicBlockTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT CopyPropagation final : public BasicBlockTransform {
public:
    bool runOnBasicBlock(BasicBlock &B) override;

    const char *name() const override {
        return "CopyPropagation";
    }
};

} // namespace mini_llvm::mir
