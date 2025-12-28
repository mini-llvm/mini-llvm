// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Type/ArrayType.h"

#include <memory>
#include <typeinfo>
#include <vector>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/ArrayConstant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::ir;

std::unique_ptr<Constant> ArrayType::zeroValue() const {
    return std::make_unique<ArrayConstant>(cast<ArrayType>(clone()), std::vector<std::shared_ptr<Constant>>{});
}

bool ArrayType::equals(const Type &other) const {
    if (typeid(*this) != typeid(other)) {
        return false;
    }
    const ArrayType &castOther = static_cast<const ArrayType &>(other);
    return *elementType() == *castOther.elementType() && numElements() == castOther.numElements();
}
