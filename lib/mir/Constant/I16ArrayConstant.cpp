#include "mini-llvm/mir/Constant/I16ArrayConstant.h"

#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string I16ArrayConstant::format() const {
    StringJoiner formatted("\n", "i16 [\n", "\n]");
    for (int16_t element : elements()) {
        formatted.addFormat("  {}", element);
    }
    return formatted.toString();
}
