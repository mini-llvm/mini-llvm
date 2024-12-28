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
    std::unordered_map<Register *, Register *> copies;

    for (Instruction &I : B) {
        for (RegisterOperand *op : I.srcs()) {
            if (auto i = copies.find(&**op); i != copies.end()) {
                op->set(share(*i->second));
                changed = true;
            }
        }
        if (auto *mov = dynamic_cast<const Mov *>(&I); mov && &*mov->dst() == &*mov->src()) {
            continue;
        }
        for (Register *reg : def(I)) {
            copies.erase(reg);
            for (auto j = copies.begin(); j != copies.end();) {
                if (j->second == reg) {
                    j = copies.erase(j);
                } else {
                    ++j;
                }
            }
        }
        if (auto *mov = dynamic_cast<const Mov *>(&I)) {
            copies[&*mov->dst()] = &*mov->src();
        }
    }

    return changed;
}
