#pragma once

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/ir/Type.h"

namespace mini_llvm::ir {

class FloatingType : public Type {
public:
    virtual Precision precision() const = 0;
};

} // namespace mini_llvm::ir
