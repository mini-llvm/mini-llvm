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
#include "mini-llvm/opt/ir/passes/PoisonPropagation.h"
#include "mini-llvm/opt/ir/passes/StrengthReduction.h"
#include "mini-llvm/opt/ir/passes/UnreachableBlockElimination.h"
#include "mini-llvm/opt/ir/passes/VerificationAnalysis.h"

using namespace mini_llvm::ir;

void PassManager::run(Module &M) const {
    Mem2Reg pass;
    run(pass, M);

    bool changed;
    do {
        changed = false;

        DeadCodeElimination         pass1;
        UnreachableBlockElimination pass2;
        JumpThreading               pass3;
        BasicBlockMerging           pass4;
        BranchSimplification        pass5;
        AlgebraicSimplification     pass6;
        ConstantFolding             pass7;
        PoisonPropagation           pass8;
        StrengthReduction           pass9(3);
        FunctionInlining            pass10;

        changed |= run(pass1, M);
        changed |= run(pass2, M);
        changed |= run(pass3, M);
        changed |= run(pass4, M);
        changed |= run(pass5, M);
        changed |= run(pass6, M);
        changed |= run(pass7, M);
        changed |= run(pass8, M);
        changed |= run(pass9, M);
        changed |= run(pass10, M);
    } while (changed);
}

bool PassManager::run(ModuleTransform &pass, Module &M) const {
    bool changed = pass.runOnModule(M);
    if (verifyAfterEach_) {
        VerificationAnalysis().runOnModule(M);
    }
    return changed;
}
