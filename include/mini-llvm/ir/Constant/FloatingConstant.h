#pragma once

#include <cstdint>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"

namespace mini_llvm::ir {

class FloatingConstant : public Constant {
public:
    virtual uint64_t bitPattern() const = 0;
};

} // namespace mini_llvm::ir
