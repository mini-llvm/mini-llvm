#include "mini-llvm/ir/Type/I1.h"

#include <memory>

#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Type.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> I1::zeroValue() const {
    return std::make_unique<I1Constant>(false);
}
