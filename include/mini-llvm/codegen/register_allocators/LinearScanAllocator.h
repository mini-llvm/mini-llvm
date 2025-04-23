#pragma once

#include <unordered_map>
#include <unordered_set>

#include "mini-llvm/codegen/RegisterAllocator.h"
#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/VirtualRegister.h"

namespace mini_llvm {

class LinearScanAllocator : public RegisterAllocator {
public:
    bool allocate(
        mir::Function &F,
        int regWidth,
        const std::unordered_set<mir::VirtualRegister *> &virtRegs,
        const std::unordered_set<mir::PhysicalRegister *> &physRegs,
        PhysicalRegisterAction load,
        PhysicalRegisterAction store,
        const std::unordered_multimap<mir::VirtualRegister *, mir::PhysicalRegister *> &hints
    ) override;
};

} // namespace mini_llvm
