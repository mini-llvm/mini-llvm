#pragma once

#include <unordered_set>

#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"
#include "mini-llvm/utils/FunctionRef.h"

namespace mini_llvm {

class RegisterAllocator {
public:
    using RegisterAction = FunctionRef<
        void (mir::Register *reg, mir::StackSlot *slot, const mir::BasicBlockBuilder &builder)
    >;

    virtual ~RegisterAllocator() = default;

    virtual bool allocate(mir::Function &F,
        const std::unordered_set<mir::VirtualRegister *> &virtRegs,
        const std::unordered_set<mir::PhysicalRegister *> &physRegs,
        RegisterAction load,
        RegisterAction store
    ) = 0;
};

} // namespace mini_llvm
