#include "mini-llvm/mir/Constant/PtrConstant.h"

#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string PtrConstant::format() const {
    StringJoiner formatted(" ");
    formatted.add("ptr({})", ptrSize());
    if (ptr() == nullptr) {
        formatted.add("null");
    } else {
        formatted.add("{:o}", *ptr());
    }
    return formatted.toString();
}
