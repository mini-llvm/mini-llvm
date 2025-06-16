#include "mini-llvm/mc/Fragment.h"

#include <string>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mc;

std::string Fragment::format() const {
    StringJoiner formatted("\n");
    if (isGlobal()) {
        formatted.add("  .globl {}", name());
    }
    if (alignment()) {
        formatted.add("  .balign {}", alignment());
    }
    formatted.add("{}:", name());
    for (const Line &line : *this) {
        if (dynamic_cast<const Instruction *>(&line)) {
            formatted.add("  {}", line);
        }
        if (dynamic_cast<const Directive *>(&line)) {
            formatted.add("  {}", line);
        }
        if (dynamic_cast<const Label *>(&line)) {
            formatted.add("{}", line);
        }
    }
    return formatted.toString();
}
