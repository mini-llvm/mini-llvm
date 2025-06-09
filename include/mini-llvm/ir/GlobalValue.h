#pragma once

#include <cstdlib>
#include <memory>
#include <string>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class GlobalValue : public Constant {
public:
    std::unique_ptr<Type> type() const override {
        return std::make_unique<Ptr>();
    }

    virtual bool isDeclaration() const = 0;

    std::string formatAsOperand() const override;

    std::unique_ptr<Value> clone() const override {
        abort();
    }
};

} // namespace mini_llvm::ir
