#include "mini-llvm/opt/ir/passes/StrengthReduction.h"

#include <cstdint>
#include <memory>
#include <vector>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/SHL.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

std::vector<std::shared_ptr<Instruction>> replaceMul(const Mul &I, size_t threshold) {
    if (dynamic_cast<const IntegerConstant *>(&*I.rhs())) {
        uint64_t rhs = dynamic_cast<const IntegerConstant *>(&*I.rhs())->zeroExtendedValue();

        if (rhs > 0) {
            std::vector<std::shared_ptr<Instruction>> replaced;
            std::vector<std::shared_ptr<Value>> sum;
            std::shared_ptr<Value> lhs = share(*I.lhs());
            if (rhs & 1) {
                sum.push_back(lhs);
            }
            for (int i = 1; i < 64; ++i) {
                if ((rhs >> i) & 1) {
                    std::shared_ptr<Instruction> shl = std::make_shared<SHL>(lhs, createIntegerConstant(lhs->type(), i));
                    replaced.push_back(shl);
                    sum.push_back(shl);
                }
            }
            size_t n = sum.size();
            if (n >= 2) {
                replaced.push_back(std::make_shared<Add>(sum[0], sum[1]));
                for (size_t i = 2; i < n; ++i) {
                    replaced.push_back(std::make_shared<Add>(replaced.back(), sum[i]));
                }
            }
            if (replaced.size() <= threshold) {
                return replaced;
            }

            return {};
        }

        return {};
    }

    return {};
}

void dfs(const DominatorTreeNode *node, bool &changed, size_t mulThreshold) {
    std::vector<const Instruction *> remove;

    for (const Instruction &I : *node->block) {
        std::vector<std::shared_ptr<Instruction>> replaced;

        if (auto *mul = dynamic_cast<const Mul *>(&I)) {
            replaced = replaceMul(*mul, mulThreshold);
        }

        if (!replaced.empty()) {
            Instruction *result = &*replaced.back();
            for (auto &II : replaced) {
                addToParent(I, std::move(II));
                changed = true;
            }
            changed |= replaceAllUsesWith(I, share(*result));
            remove.push_back(&I);
        }
    }

    for (const Instruction *I : remove) {
        removeFromParent(*I);
        changed = true;
    }

    for (const DominatorTreeNode *child : node->children) {
        dfs(child, changed, mulThreshold);
    }
}

} // namespace

bool StrengthReduction::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        for (Instruction &I : B) {
            if (auto *op = dynamic_cast<BinaryIntegerArithmeticOperator *>(&I)) {
                if (op->isCommutative()
                        && dynamic_cast<const IntegerConstant *>(&*op->lhs()) && !dynamic_cast<const IntegerConstant *>(&*op->rhs())) {
                    std::shared_ptr<Value> lhs = share(*op->lhs()),
                                           rhs = share(*op->rhs());
                    op->lhs().set(std::move(rhs));
                    op->rhs().set(std::move(lhs));
                    changed = true;
                }
            }
        }
    }

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    dfs(domTree.node(F.entry()), changed, mulThreshold_);

    return changed;
}
