#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVFallthrough.h"

#include <iterator>

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/mc/LabelOperand.h"
#include "mini-llvm/mc/Section.h"
#include "mini-llvm/targets/riscv/mc/RISCVInstruction.h"
#include "mini-llvm/targets/riscv/mc/RISCVOperation.h"

using namespace mini_llvm::mc;

bool RISCVFallthrough::runOnFragment(Fragment &fragment) {
    if (fragment.section() != Section::kText) {
        return false;
    }

    bool changed = false;

    for (Fragment::const_iterator i = fragment.begin(); std::next(i) != fragment.end();) {
        if (auto *I = dynamic_cast<const RISCVInstruction *>(&*i)) {
            if (I->opcode() == RISCV_J) {
                if (auto *labelOp = dynamic_cast<const LabelOperand *>(&*I->operand_begin())) {
                    if (auto *label = dynamic_cast<const Label *>(&*std::next(i))) {
                        if (labelOp->labelName() == label->labelName()) {
                            fragment.remove(i++);
                            changed = true;
                            continue;
                        }
                    }
                }
            }
        }
        ++i;
    }

    return changed;
}
