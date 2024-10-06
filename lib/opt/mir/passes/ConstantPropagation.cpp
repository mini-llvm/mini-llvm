#include "mini-llvm/opt/mir/passes/ConstantPropagation.h"

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Immediate.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/LI.h"
#include "mini-llvm/mir/Instruction/Mov.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm::mir;

bool ConstantPropagation::runOnBasicBlock(BasicBlock &B) {
    std::unordered_map<Register *, std::unique_ptr<Immediate>> values;
    std::vector<std::pair<BasicBlock::const_iterator, std::unique_ptr<Instruction>>> replace;

    for (BasicBlock::const_iterator i = B.begin(); i != B.end(); ++i) {
        for (Register *reg : def(*i)) {
            values.erase(reg);
        }
        if (auto *li = dynamic_cast<const LI *>(&*i)) {
            values[&*li->dst()] = li->src()->clone();
        }
        if (auto *mov = dynamic_cast<const Mov *>(&*i)) {
            if (values.contains(&*mov->src())) {
                replace.emplace_back(i, std::make_unique<LI>(mov->width(), share(*mov->dst()), values[&*mov->src()]->clone()));
            }
        }
    }

    bool changed = false;

    for (auto &[i, I] : replace) {
        B.add(i, std::move(I));
        B.remove(i);
        changed = true;
    }

    return changed;
}
