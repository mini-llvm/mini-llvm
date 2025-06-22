#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVFallthrough.h"

#include <iterator>

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/mc/LabelOperand.h"
#include "mini-llvm/mc/Section.h"
#include "mini-llvm/targets/riscv/mc/RISCVInstruction.h"
#include "mini-llvm/targets/riscv/mc/RISCVOperation.h"

using namespace mini_llvm::mc;

bool RISCVFallthrough::runOnGlobalValue(GlobalValue &G) {
    if (G.section() != Section::kText) {
        return false;
    }

    bool changed = false;

    for (GlobalValue::const_iterator i = G.begin(); std::next(i) != G.end();) {
        if (auto *I = dynamic_cast<const RISCVInstruction *>(&*i)) {
            if (I->opcode() == RISCV_J) {
                if (auto *labelOp = dynamic_cast<const LabelOperand *>(&*I->operand_begin())) {
                    if (auto *label = dynamic_cast<const Label *>(&*std::next(i))) {
                        if (labelOp->labelName() == label->labelName()) {
                            G.remove(i++);
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
