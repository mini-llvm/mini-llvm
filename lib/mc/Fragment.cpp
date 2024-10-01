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
        formatted.addFormat("  .globl {}", name());
    }
    formatted.addFormat("{}:", name());
    for (const Line &line : *this) {
        if (dynamic_cast<const Instruction *>(&line)) {
            formatted.addFormat("  {}", line);
        }
        if (dynamic_cast<const Directive *>(&line)) {
            formatted.addFormat("  {}", line);
        }
        if (dynamic_cast<const Label *>(&line)) {
            formatted.addFormat("{}", line);
        }
    }
    return formatted.toString();
}
