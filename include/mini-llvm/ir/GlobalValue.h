// SPDX-License-Identifier: MIT

#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include "mini-llvm/ir/Constant/PointerConstant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT GlobalValue : public PointerConstant {
public:
    std::unique_ptr<Type> type() const override {
        return std::make_unique<Ptr>();
    }

    virtual bool isDeclaration() const = 0;

    std::string formatAsOperand() const override {
        return "@" + formatName();
    }

    std::unique_ptr<Value> clone() const override {
        abort();
    }
};

} // namespace mini_llvm::ir
