#include "mini-llvm/mir/Constant/I64ArrayConstant.h"

#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string I64ArrayConstant::format() const {
    StringJoiner formatted("\n", "i64 [\n", "\n]");
    for (int64_t element : elements()) {
        formatted.addFormat("  {}", element);
    }
    return formatted.toString();
}
