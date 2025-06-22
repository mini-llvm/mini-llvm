#include "mini-llvm/opt/mc/passes/UnusedLabelElimination.h"

#include <string>
#include <unordered_set>

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/mc/LabelOperand.h"
#include "mini-llvm/mc/Line.h"
#include "mini-llvm/mc/Section.h"

using namespace mini_llvm::mc;

bool UnusedLabelElimination::runOnFragment(Fragment &fragment) {
    if (fragment.section() != Section::kText) {
        return false;
    }

    std::unordered_set<std::string> used;

    for (const Line &line : fragment) {
        if (auto *I = dynamic_cast<const Instruction *>(&line)) {
            for (const Operand &op : operands(*I)) {
                if (auto *labelOp = dynamic_cast<const LabelOperand *>(&op)) {
                    used.insert(labelOp->labelName());
                }
            }
        }
    }

    bool changed = false;

    for (Fragment::const_iterator i = fragment.begin(); i != fragment.end();) {
        if (auto *label = dynamic_cast<const Label *>(&*i)) {
            if (!used.contains(label->labelName())) {
                fragment.remove(i++);
                changed = true;
                continue;
            }
        }
        ++i;
    }

    return changed;
}
