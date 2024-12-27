#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/opt/mir/BasicBlockTransform.h"

namespace mini_llvm::mir {

class CopyPropagation final : public BasicBlockTransform {
public:
    bool runOnBasicBlock(BasicBlock &B) override;
};

} // namespace mini_llvm::mir
