#pragma once

#include <unordered_set>

#include "mini-llvm/codegen/RegisterAllocator.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/VirtualRegister.h"

namespace mini_llvm {

class GraphColoringAllocator : public RegisterAllocator {
public:
    bool allocate(
        mir::Function &F,
        const std::unordered_set<mir::VirtualRegister *> &virtRegs,
        const std::unordered_set<mir::PhysicalRegister *> &physRegs,
        RegisterAction load,
        RegisterAction store
    ) override;
};

} // namespace mini_llvm
