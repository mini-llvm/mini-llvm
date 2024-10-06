#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"

namespace mini_llvm::ir {

class PassManager {
public:
    void setVerifyAfterEach(bool verifyAfterEach) {
        verifyAfterEach_ = verifyAfterEach;
    }

    void run(Module &M) const;

private:
    bool verifyAfterEach_ = false;

    bool run(ModuleTransform &pass, Module &M) const;
};

} // namespace mini_llvm::ir
