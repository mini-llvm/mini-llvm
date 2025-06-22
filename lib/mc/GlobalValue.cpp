#include "mini-llvm/mc/GlobalValue.h"

#include <string>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mc;

std::string GlobalValue::format() const {
    StringJoiner formatted("\n");
    if (isGlobal()) {
        formatted.add("  .globl {}", name());
    }
    if (alignment()) {
        formatted.add("  .balign {}", alignment());
    }
    formatted.add("{}:", name());
    for (const Statement &stmt : *this) {
        if (dynamic_cast<const Instruction *>(&stmt)) {
            formatted.add("  {}", stmt);
        }
        if (dynamic_cast<const Directive *>(&stmt)) {
            formatted.add("  {}", stmt);
        }
        if (dynamic_cast<const Label *>(&stmt)) {
            formatted.add("{}", stmt);
        }
    }
    return formatted.toString();
}
