#pragma once

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/GlobalVar.h"
#include "mini-llvm/ir/Module.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/GlobalVar.h"
#include "mini-llvm/mir/Module.h"
#include "mini-llvm/utils/HashMap.h"

namespace mini_llvm {

class RISCVMIRGen {
public:
    static constexpr int kSave = 0,
                         kRestore = 1;

    RISCVMIRGen(const ir::Module *IM, mir::Module *MM) : IM_(IM), MM_(MM) {}

    void emit();

private:
    const ir::Module *IM_;
    mir::Module *MM_;
    HashMap<const ir::GlobalVar *, mir::GlobalVar *> globalVarMap_;
    HashMap<const ir::Function *, mir::Function *> functionMap_;

    void emitGlobalVar(const ir::GlobalVar &IG, mir::GlobalVar &MG);
    void emitFunction(const ir::Function &IF, mir::Function &MF);
};

} // namespace mini_llvm
