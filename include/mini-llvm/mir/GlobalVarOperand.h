#pragma once

#include "mini-llvm/mir/GlobalVar.h"

namespace mini_llvm::mir {

class GlobalVarOperand {
public:
    explicit GlobalVarOperand(GlobalVar *globalVar) : globalVar_(globalVar) {}

    GlobalVar &operator*() const {
        return *globalVar_;
    }

    GlobalVar *operator->() const {
        return globalVar_;
    }

    void set(GlobalVar *globalVar) {
        globalVar_ = globalVar;
    }

private:
    GlobalVar *globalVar_;
};

} // namespace mini_llvm::mir
