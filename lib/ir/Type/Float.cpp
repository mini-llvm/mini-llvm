#include "mini-llvm/ir/Type/Float.h"

#include <memory>

#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> Float::zeroValue() const {
    return std::make_unique<FloatConstant>(0.f);
}
