#pragma once

#include <string>
#include <unordered_map>

#include "mini-llvm/mc/Fragment.h"
#include "mini-llvm/mc/Program.h"
#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/mir/Module.h"

namespace mini_llvm {

class RISCVMCGen {
public:
    RISCVMCGen(const mir::Module *MM, mc::Program *program) : MM_(MM), program_(program) {}

    void emit();

private:
    const mir::Module *MM_;
    mc::Program *program_;
    std::unordered_map<const mir::BasicBlock *, std::string> labelMap_;

    void emitGlobalVar(const mir::GlobalVar &G, mc::Fragment &fragment);
    void emitFunction(const mir::Function &F, mc::Fragment &fragment);
};

} // namespace mini_llvm
