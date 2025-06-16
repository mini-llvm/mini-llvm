#include "mini-llvm/mir/Constant/PtrArrayConstant.h"

#include <format>
#include <string>

#include "mini-llvm/mir/GlobalValue.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string PtrArrayConstant::format() const {
    StringJoiner formattedElements("\n", "[\n", "\n]");
    for (GlobalValue *element : elements()) {
        if (element == nullptr) {
            formattedElements.add("  null");
        } else {
            formattedElements.add("  {:o}", *element);
        }
    }
    return std::format("ptr({}) {}", ptrSize(), formattedElements);
}
