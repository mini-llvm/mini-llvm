#include "mini-llvm/opt/ir/PassManager.h"

#include "mini-llvm/ir/Module.h"
#include "mini-llvm/opt/ir/ModuleTransform.h"
#include "mini-llvm/opt/ir/passes/AlgebraicSimplification.h"
#include "mini-llvm/opt/ir/passes/BasicBlockMerging.h"
#include "mini-llvm/opt/ir/passes/BranchSimplification.h"
#include "mini-llvm/opt/ir/passes/ConstantFolding.h"
#include "mini-llvm/opt/ir/passes/DeadCodeElimination.h"
#include "mini-llvm/opt/ir/passes/FunctionInlining.h"
#include "mini-llvm/opt/ir/passes/JumpThreading.h"
#include "mini-llvm/opt/ir/passes/Mem2Reg.h"
#include "mini-llvm/opt/ir/passes/PoisonFreeze.h"
#include "mini-llvm/opt/ir/passes/PoisonPropagation.h"
#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

using namespace mini_llvm::ir;

void PassManager::run(Module &M) const {
    Mem2Reg pass;
    run(pass, M);

    bool changed;
    do {
        changed = false;

        bool changed2;
        do {
            changed2 = false;

            DeadCodeElimination         pass1;
            JumpThreading               pass2;
            UnreachableBlockElimination pass3;
            AlgebraicSimplification     pass4;
            ConstantFolding             pass5;
            PoisonPropagation           pass6;
            BranchSimplification        pass7;
            BasicBlockMerging           pass8;
            FunctionInlining            pass9;

            changed2 |= run(pass1, M);
            changed2 |= run(pass2, M);
            changed2 |= run(pass3, M);
            changed2 |= run(pass4, M);
            changed2 |= run(pass5, M);
            changed2 |= run(pass6, M);
            changed2 |= run(pass7, M);
            changed2 |= run(pass8, M);
            changed2 |= run(pass9, M);

            changed |= changed2;
        } while (changed2);

        PoisonFreeze pass;

        changed |= run(pass, M);
    } while (changed);
}

bool PassManager::run(ModuleTransform &pass, Module &M) const {
    bool changed = pass.runOnModule(M);
    if (verifyAfterEach_) {
        VerificationAnalysis pass2;
        pass2.runOnModule(M);
    }
    return changed;
}
