#pragma once

#include <memory>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"

namespace mini_llvm::ir {

class FloatingConstant : public Constant {
public:
    virtual uint64_t bitPattern() const = 0;
};

std::unique_ptr<FloatingConstant> createFloatingConstant(std::unique_ptr<Type> type, double value);

} // namespace mini_llvm::ir
