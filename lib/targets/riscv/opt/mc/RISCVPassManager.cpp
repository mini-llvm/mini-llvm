#include "mini-llvm/targets/riscv/opt/mc/RISCVPassManager.h"

#include "mini-llvm/mc/Program.h"
#include "mini-llvm/opt/mc/passes/UnusedLabelElimination.h"
#include "mini-llvm/targets/riscv/opt/mc/passes/RISCVFallthrough.h"

using namespace mini_llvm::mc;

void RISCVPassManager::run(Program &program) const {
    RISCVFallthrough().runOnProgram(program);
    UnusedLabelElimination().runOnProgram(program);
}
