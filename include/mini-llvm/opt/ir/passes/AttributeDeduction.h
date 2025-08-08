// SPDX-License-Identifier: MIT

#pragma once

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT AttributeDeduction final : public ModuleTransform {
public:
    bool runOnModule(Module &M) override;

    const char *name() const override {
        return "AttributeDeduction";
    }
};

} // namespace mini_llvm::ir
