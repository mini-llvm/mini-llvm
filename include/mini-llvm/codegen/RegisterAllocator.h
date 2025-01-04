#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlockBuilder.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/StackSlot.h"
#include "mini-llvm/mir/VirtualRegister.h"

namespace mini_llvm {

class RegisterAllocator {
public:
    virtual ~RegisterAllocator() = default;

    virtual bool allocate(mir::Function &F,
                          int regWidth,
                          const std::unordered_set<mir::VirtualRegister *> &virtRegs,
                          const std::unordered_set<mir::PhysicalRegister *> &physRegs,
                          std::function<void (mir::PhysicalRegister *physReg, mir::StackSlot *slot, const mir::BasicBlockBuilder &builder)> load,
                          std::function<void (mir::PhysicalRegister *physReg, mir::StackSlot *slot, const mir::BasicBlockBuilder &builder)> store,
                          const std::unordered_multimap<mir::VirtualRegister *, mir::PhysicalRegister *> &hints) = 0;
};

} // namespace mini_llvm
