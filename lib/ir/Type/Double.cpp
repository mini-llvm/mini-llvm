#include "mini-llvm/ir/Type/Double.h"

#include <memory>

#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> Double::zeroValue() const {
    return std::make_unique<DoubleConstant>(0.);
}
