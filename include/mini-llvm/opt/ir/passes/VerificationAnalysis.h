#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/FunctionAnalysis.h"
#include "mini-llvm/opt/ir/ModuleAnalysis.h"

namespace mini_llvm::ir {

class VerificationAnalysis final : public FunctionAnalysis, public ModuleAnalysis {
public:
    void runOnFunction(const Function &F) override;
    void runOnModule(const Module &M) override;

    bool ok() const {
        return ok_;
    }

private:
    bool ok_;
};

} // namespace mini_llvm::ir
