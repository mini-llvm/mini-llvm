#include "mini-llvm/opt/mc/passes/RedundantLabelElimination.h"

#include <string>
#include <unordered_set>
#include <vector>

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/mc/LabelOperand.h"
#include "mini-llvm/mc/Line.h"
#include "mini-llvm/mc/Section.h"

using namespace mini_llvm::mc;

bool RedundantLabelElimination::runOnFragment(Fragment &fragment) {
    if (fragment.section() != Section::kText) return false;

    std::unordered_set<std::string> referenced;

    for (const Line &line : fragment) {
        if (auto *I = dynamic_cast<const Instruction *>(&line)) {
            for (const Operand &op : operands(*I)) {
                if (auto *labelOp = dynamic_cast<const LabelOperand *>(&op)) {
                    referenced.insert(labelOp->labelName());
                }
            }
        }
    }

    std::vector<Fragment::const_iterator> remove;

    for (Fragment::const_iterator i = fragment.begin(), e = fragment.end(); i != e; ++i) {
        if (auto *label = dynamic_cast<const Label *>(&*i)) {
            if (!referenced.contains(label->labelName())) {
                remove.push_back(i);
            }
        }
    }

    bool changed = false;

    for (Fragment::const_iterator i : remove) {
        fragment.remove(i);
        changed = true;
    }

    return changed;
}
