#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/opt/mir/BasicBlockTransform.h"

namespace mini_llvm::mir {

class ZeroRegisterReplacement final : public BasicBlockTransform {
public:
    explicit ZeroRegisterReplacement(PhysicalRegister *zeroReg) : zeroReg_(zeroReg) {}

    bool runOnBasicBlock(BasicBlock &B) override;

private:
    PhysicalRegister *zeroReg_;
};

} // namespace mini_llvm::mir
