// SPDX-License-Identifier: MIT

#include "mini-llvm/opt/ir/passes/InstructionCombining.h"

#include <cassert>
#include <memory>
#include <utility>
#include <vector>

#include "mini-llvm/common/ops/Add.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/IntegerConstant.h"
#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Instruction/Add.h"
#include "mini-llvm/ir/Instruction/BinaryIntegerArithmeticOperator.h"
#include "mini-llvm/ir/Instruction/GetElementPtr.h"
#include "mini-llvm/ir/Instruction/Mul.h"
#include "mini-llvm/ir/Instruction/Sub.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/opt/ir/passes/DominatorTreeAnalysis.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

bool isCombinableGEP(const Value &value) {
    if (auto *gep = dynamic_cast<const GetElementPtr *>(&value)) {
        if (gep->idx_size() == 1 && dynamic_cast<const Constant *>(&**gep->idx_begin())) {
            return true;
        }
    }
    return false;
}

void dfs(const DTNode *node, bool &changed) {
    for (auto i = const_cast<BasicBlock *>(node->block)->begin(); i != const_cast<BasicBlock *>(node->block)->end();) {
        Instruction &I = *i++;
        if (dynamic_cast<const Add *>(&I) && dynamic_cast<const Constant *>(&*static_cast<const Add *>(&I)->rhs())) {
            Add *add = static_cast<Add *>(&I);
            std::shared_ptr<Value> lhs = share(*add->lhs()),
                                   rhs = share(*add->rhs());
            while (dynamic_cast<const Add *>(&*lhs) && dynamic_cast<const Constant *>(&*static_cast<const Add *>(&*lhs)->rhs())) {
                Add *add2 = static_cast<Add *>(&*lhs);
                lhs = share(*add2->lhs());
                rhs = Add(rhs, share(*add2->rhs())).fold();
            }
            if (&*lhs != &*add->lhs()) {
                std::shared_ptr<Instruction> II = std::make_shared<Add>(lhs, rhs);
                addToParent(I, II);
                replaceAllUsesWith(I, II);
                removeFromParent(I);
                changed = true;
            }
            continue;
        }
        if (dynamic_cast<const Mul *>(&I) && dynamic_cast<const Constant *>(&*static_cast<const Mul *>(&I)->rhs())) {
            Mul *mul = static_cast<Mul *>(&I);
            std::shared_ptr<Value> lhs = share(*mul->lhs()),
                                   rhs = share(*mul->rhs());
            while (dynamic_cast<const Mul *>(&*lhs) && dynamic_cast<const Constant *>(&*static_cast<const Mul *>(&*lhs)->rhs())) {
                Mul *mul2 = static_cast<Mul *>(&*lhs);
                lhs = share(*mul2->lhs());
                rhs = Mul(rhs, share(*mul2->rhs())).fold();
            }
            if (&*lhs != &*mul->lhs()) {
                std::shared_ptr<Instruction> II = std::make_shared<Mul>(lhs, rhs);
                addToParent(I, II);
                replaceAllUsesWith(I, II);
                removeFromParent(I);
                changed = true;
            }
            continue;
        }
        if (isCombinableGEP(I)) {
            GetElementPtr *gep = static_cast<GetElementPtr *>(&I);
            std::shared_ptr<Value> ptr = share(*gep->ptr()),
                                   idx = share(**gep->idx_begin());
            while (isCombinableGEP(*ptr) && *static_cast<const GetElementPtr *>(&*ptr)->sourceType() == *gep->sourceType()) {
                GetElementPtr *gep2 = static_cast<GetElementPtr *>(&*ptr);
                ptr = share(*gep2->ptr());
                idx = std::make_shared<I64Constant>(ops::Add()(
                    static_cast<const IntegerConstant *>(&*idx)->signExtendedValue(),
                    static_cast<const IntegerConstant *>(&**gep2->idx_begin())->signExtendedValue()
                ));
            }
            if (&*ptr != &*gep->ptr()) {
                std::vector<std::shared_ptr<Value>> indices;
                indices.push_back(idx);
                std::shared_ptr<Instruction> II = std::make_shared<GetElementPtr>(gep->sourceType(), ptr, indices);
                addToParent(I, II);
                replaceAllUsesWith(I, II);
                removeFromParent(I);
                changed = true;
            }
            continue;
        }
    }

    for (const DTNode *child : node->children) {
        dfs(child, changed);
    }
}

} // namespace

bool InstructionCombining::runOnFunction(Function &F) {
    bool changed = false;

    for (BasicBlock &B : F) {
        for (auto i = B.begin(); i != B.end();) {
            Instruction &I = *i++;
            if (auto *op = dynamic_cast<BinaryIntegerArithmeticOperator *>(&I)) {
                if (op->isCommutative()
                        && dynamic_cast<const Constant *>(&*op->lhs())
                        && !dynamic_cast<const Constant *>(&*op->rhs())) {
                    std::shared_ptr<Value> lhs = share(*op->lhs()),
                                           rhs = share(*op->rhs());
                    op->lhs().set(std::move(rhs));
                    op->rhs().set(std::move(lhs));
                    changed = true;
                }
            }
            if (auto *sub = dynamic_cast<const Sub *>(&I)) {
                if (dynamic_cast<const Constant *>(&*sub->rhs())) {
                    std::shared_ptr<Instruction> add = std::make_shared<Add>(
                        share(*sub->lhs()), Sub(sub->rhs()->type()->constant(0), share(*sub->rhs())).fold()
                    );
                    addToParent(*sub, add);
                    replaceAllUsesWith(*sub, add);
                    removeFromParent(*sub);
                    changed = true;
                }
            }
        }
    }

    DominatorTreeAnalysis domTree;
    domTree.runOnFunction(F);

    dfs(domTree.node(F.entry()), changed);

    assert(F.isWellFormed());
    return changed;
}
