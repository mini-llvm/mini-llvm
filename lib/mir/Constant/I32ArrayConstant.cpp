#include "mini-llvm/mir/Constant/I32ArrayConstant.h"

#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string I32ArrayConstant::format() const {
    StringJoiner formatted("\n", "i32 [\n", "\n]");
    for (int32_t element : elements()) {
        formatted.addFormat("  {}", element);
    }
    return formatted.toString();
}
