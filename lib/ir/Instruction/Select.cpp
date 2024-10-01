#include "mini-llvm/ir/Instruction/Select.h"

#include <memory>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> Select::fold() const {
    if (dynamic_cast<const PoisonValue *>(&*cond())) {
        return std::make_unique<PoisonValue>(type());
    }
    if (static_cast<const I1Constant *>(&*cond())->value() == true) {
        return cast<Constant>(trueValue()->clone());
    } else {
        return cast<Constant>(falseValue()->clone());
    }
}
