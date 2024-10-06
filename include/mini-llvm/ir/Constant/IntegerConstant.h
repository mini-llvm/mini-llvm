#pragma once

#include <cstdint>
#include <memory>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"

namespace mini_llvm::ir {

class IntegerConstant : public Constant {
public:
    virtual int64_t signExtendedValue() const = 0;
    virtual int64_t zeroExtendedValue() const = 0;
};

std::unique_ptr<IntegerConstant> createIntegerConstant(std::unique_ptr<Type> type, int64_t value);

} // namespace mini_llvm::ir
