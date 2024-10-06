#pragma once

#include <unordered_map>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/mir/Module.h"

namespace mini_llvm {

class RISCVMIRGen {
public:
    RISCVMIRGen(const ir::Module *IM, mir::Module *MM) : IM_(IM), MM_(MM) {}

    void emit();

private:
    const ir::Module *IM_;
    mir::Module *MM_;
    std::unordered_map<const ir::GlobalVar *, mir::GlobalVar *> globalVarMap_;
    std::unordered_map<const ir::Function *, mir::Function *> functionMap_;

    void emitGlobalVar(const ir::GlobalVar &G, mir::GlobalVar &m_G);
    void emitFunction(const ir::Function &F, mir::Function &m_F);
};

} // namespace mini_llvm
