#include "mini-llvm/mir/Constant/I8ArrayConstant.h"

#include <cstdint>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string I8ArrayConstant::format() const {
    StringJoiner formatted("\n", "i8 [\n", "\n]");
    for (int8_t element : elements()) {
        formatted.add("  {}", element);
    }
    return formatted.toString();
}
