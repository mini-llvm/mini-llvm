#include "mini-llvm/opt/mc/passes/UnusedLabelElimination.h"

#include <string>
#include <unordered_set>

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/mc/LabelOperand.h"
#include "mini-llvm/mc/Statement.h"

using namespace mini_llvm::mc;

bool UnusedLabelElimination::runOnGlobalValue(GlobalValue &G) {
    if (G.section() != ".text") {
        return false;
    }

    std::unordered_set<std::string> used;

    for (const Statement &stmt : G) {
        if (auto *I = dynamic_cast<const Instruction *>(&stmt)) {
            for (const Operand &op : operands(*I)) {
                if (auto *labelOp = dynamic_cast<const LabelOperand *>(&op)) {
                    used.insert(labelOp->labelName());
                }
            }
        }
    }

    bool changed = false;

    for (GlobalValue::const_iterator i = G.begin(); i != G.end();) {
        if (auto *label = dynamic_cast<const Label *>(&*i)) {
            if (!used.contains(label->labelName())) {
                G.remove(i++);
                changed = true;
                continue;
            }
        }
        ++i;
    }

    return changed;
}
