// SPDX-License-Identifier: MIT

#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVFallthrough.h"

#include <iterator>

#include "mini-llvm/mc/GlobalValue.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/targets/riscv/mc/RISCVAddressOperand.h"
#include "mini-llvm/targets/riscv/mc/RISCVInstruction.h"
#include "mini-llvm/targets/riscv/mc/RISCVOperation.h"

using namespace mini_llvm::mc;

bool RISCVFallthrough::runOnGlobalValue(GlobalValue &G) {
    if (G.section() != ".text") {
        return false;
    }

    bool changed = false;

    for (GlobalValue::const_iterator i = G.begin(); std::next(i) != G.end();) {
        if (auto *I = dynamic_cast<const RISCVInstruction *>(&*i)) {
            if (I->opcode() == RISCV_J) {
                if (auto *addrOp = dynamic_cast<const RISCVAddressOperand *>(&*I->operand_begin())) {
                    if (auto *label = dynamic_cast<const Label *>(&*std::next(i))) {
                        if (addrOp->addr().baseSymbol() == label->symbol()) {
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
