#pragma once

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/opt/mir/BasicBlockTransform.h"

namespace mini_llvm::mir {

class NullOperationElimination final : public BasicBlockTransform {
public:
    explicit NullOperationElimination(int regWidth) : regWidth_(regWidth) {}

    bool runOnBasicBlock(BasicBlock &B) override;

    const char *name() const override {
        return "NullOperationElimination";
    }

private:
    int regWidth_;
};

} // namespace mini_llvm::mir
