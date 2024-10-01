#include "mini-llvm/mir/GlobalVar.h"

#include <format>
#include <string>

#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

std::string GlobalVar::format() const {
    if (!hasInitializer()) {
        return std::format("undef {:o}", *this);
    }
    StringJoiner formatted(" ");
    if (linkage() == Linkage::kInternal) {
        formatted.add("internal");
    }
    formatted.addFormat("{:o}: {}", *this, initializer());
    return formatted.toString();
}
