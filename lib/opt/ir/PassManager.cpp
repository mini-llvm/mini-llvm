#include "mini-llvm/opt/ir/PassManager.h"

#include <cassert>

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/ir/Verify.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"
#include "mini-llvm/opt/ir/passes/AlgebraicSimplification.h"
#include "mini-llvm/opt/ir/passes/ArrayFlattening.h"
#include "mini-llvm/opt/ir/passes/AttributeDeduction.h"
#include "mini-llvm/opt/ir/passes/BasicBlockMerging.h"
#include "mini-llvm/opt/ir/passes/BranchSimplification.h"
#include "mini-llvm/opt/ir/passes/ConstantFolding.h"
#include "mini-llvm/opt/ir/passes/DeadCodeElimination.h"
#include "mini-llvm/opt/ir/passes/DeadStoreElimination.h"
#include "mini-llvm/opt/ir/passes/FunctionInlining.h"
#include "mini-llvm/opt/ir/passes/GlobalCodeMotion.h"
#include "mini-llvm/opt/ir/passes/GlobalValueNumbering.h"
#include "mini-llvm/opt/ir/passes/InstructionCombining.h"
#include "mini-llvm/opt/ir/passes/JumpThreading.h"
#include "mini-llvm/opt/ir/passes/Mem2Reg.h"
#include "mini-llvm/opt/ir/passes/PoisonPropagation.h"
#include "mini-llvm/opt/ir/passes/RedundantLoadElimination.h"
#include "mini-llvm/opt/ir/passes/StrengthReduction.h"
#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"

using namespace mini_llvm::ir;

void PassManager::run(Module &M) const {
    assert(verifyModule(M));

    Mem2Reg pass0;

    pass0.runOnModule(M);
    assert(verifyModule(M));

    bool changed;
    do {
        changed = false;

        AttributeDeduction          pass1;
        DeadCodeElimination         pass2;
        BranchSimplification        pass3;
        JumpThreading               pass4;
        BasicBlockMerging           pass5;
        UnreachableBlockElimination pass6;
        RedundantLoadElimination    pass7;
        DeadStoreElimination        pass8;
        ArrayFlattening             pass9;
        InstructionCombining        pass10;
        AlgebraicSimplification     pass11;
        ConstantFolding             pass12;
        PoisonPropagation           pass13;
        GlobalCodeMotion            pass14;
        GlobalValueNumbering        pass15;
        StrengthReduction           pass16(3, 20, 20);
        FunctionInlining            pass17;

        ModuleTransform *passes[] = {
            &pass1,
            &pass2,
            &pass3,
            &pass4,
            &pass5,
            &pass6,
            &pass7,
            &pass8,
            &pass9,
            &pass10,
            &pass11,
            &pass12,
            &pass13,
            &pass14,
            &pass15,
            &pass16,
            &pass17,
        };

        for (ModuleTransform *pass : passes) {
            changed |= pass->runOnModule(M);
            assert(verifyModule(M));
        }
    } while (changed);
}
