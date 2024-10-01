#include "mini-llvm/ir/Type/FunctionType.h"

#include <memory>
#include <ranges>
#include <string>
#include <typeinfo>

#include "mini-llvm/ir/Type.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

std::string FunctionType::format() const {
    StringJoiner formatted(" ");
    formatted.addFormat("{}", *returnType());
    StringJoiner formattedParamTypes(", ", "(", ")");
    for (const Type &paramType : paramTypes(*this)) {
        formattedParamTypes.addFormat("{}", paramType);
    }
    if (isVarArgs()) {
        formatted.add("...");
    }
    formatted.addFormat("{}", formattedParamTypes);
    return formatted.toString();
}

std::unique_ptr<Type> FunctionType::clone() const {
    std::vector<std::unique_ptr<Type>> clonedParamTypes;
    for (const Type &paramType : paramTypes(*this)) {
        clonedParamTypes.push_back(paramType.clone());
    }
    return std::make_unique<FunctionType>(returnType(), std::move(clonedParamTypes), isVarArgs());
}

bool FunctionType::equals(const Type &other) const {
    if (typeid(*this) != typeid(other)) {
        return false;
    }
    const FunctionType &castOther = static_cast<const FunctionType &>(other);
    if (*returnType() != *castOther.returnType()) {
        return false;
    }
    if (param_type_size() != castOther.param_type_size()) {
        return false;
    }
    for (auto [param1, param2] : std::views::zip(paramTypes(*this), paramTypes(castOther))) {
        if (param1 != param2) {
            return false;
        }
    }
    return true;
}
