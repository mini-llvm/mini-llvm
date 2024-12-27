#include "mini-llvm/opt/mir/passes/CopyPropagation.h"

#include <unordered_map>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Mov.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;

bool CopyPropagation::runOnBasicBlock(BasicBlock &B) {
    bool changed = false;
    std::unordered_map<Register *, Register *> replace;

    for (Instruction &I : B) {
        for (RegisterOperand *op : I.srcs()) {
            if (auto i = replace.find(&**op); i != replace.end()) {
                op->set(share(*i->second));
                changed = true;
            }
        }
        for (Register *reg : def(I)) {
            replace.erase(reg);
            for (auto j = replace.begin(); j != replace.end();) {
                if (j->second == reg) {
                    j = replace.erase(j);
                } else {
                    ++j;
                }
            }
        }
        if (auto *mov = dynamic_cast<const Mov *>(&I)) {
            Register *dst = &*mov->dst(),
                     *src = &*mov->src();
            if (dst != src) {
                replace[dst] = src;
            }
        }
    }

    return changed;
}
