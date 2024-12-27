#include "mini-llvm/targets/riscv/opt/mir/RISCVPassManager.h"

#include "mini-llvm/mir/Module.h"
#include "mini-llvm/opt/mir/passes/ConstantPropagation.h"
#include "mini-llvm/opt/mir/passes/CopyPropagation.h"
#include "mini-llvm/opt/mir/passes/DeadCodeElimination.h"
#include "mini-llvm/opt/mir/passes/NullOperationElimination.h"
#include "mini-llvm/opt/mir/passes/RegisterReuse.h"
#include "mini-llvm/opt/mir/passes/ZeroRegisterReplacement.h"
#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

using namespace mini_llvm::mir;
using namespace mini_llvm::mir::riscv;

void RISCVPassManager::runBeforeRegisterAllocation(Module &M) const {
    bool changed;
    do {
        changed = false;

        NullOperationElimination pass1;
        ConstantPropagation      pass2;
        RegisterReuse            pass3;
        CopyPropagation          pass4;
        ZeroRegisterReplacement  pass5(x0());
        DeadCodeElimination      pass6;

        changed |= pass1.runOnModule(M);
        changed |= pass2.runOnModule(M);
        changed |= pass3.runOnModule(M);
        changed |= pass4.runOnModule(M);
        changed |= pass5.runOnModule(M);
        changed |= pass6.runOnModule(M);
    } while (changed);
}

void RISCVPassManager::runAfterRegisterAllocation(Module &M) const {
    bool changed;
    do {
        changed = false;

        NullOperationElimination pass1;
        ConstantPropagation      pass2;
        RegisterReuse            pass3;
        CopyPropagation          pass4;
        ZeroRegisterReplacement  pass5(x0());
        DeadCodeElimination      pass6;

        changed |= pass1.runOnModule(M);
        changed |= pass2.runOnModule(M);
        changed |= pass3.runOnModule(M);
        changed |= pass4.runOnModule(M);
        changed |= pass5.runOnModule(M);
        changed |= pass6.runOnModule(M);
    } while (changed);
}
