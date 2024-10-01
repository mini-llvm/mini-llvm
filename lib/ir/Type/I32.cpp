#include "mini-llvm/ir/Type/I32.h"

#include <memory>

#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> I32::zeroValue() const {
    return std::make_unique<I32Constant>(0);
}
