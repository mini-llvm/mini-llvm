#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

#include <cstdlib>
#include <memory>
#include <ranges>
#include <unordered_set>

#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm;
using namespace mini_llvm::mir;
using namespace mini_llvm::mir::riscv;

RISCVRegister *RISCVRegister::get(int idx) {
    static HashMap<int, std::shared_ptr<RISCVRegister>> pool;

    if (auto i = pool.find(idx); i != pool.end()) {
        return &*i->second;
    }

    switch (idx) {
#define REGS
#define X(idx, name, class, isPreserved, isAllocatable) \
    case idx: \
        return &*pool.put(idx, std::shared_ptr<RISCVRegister>(new RISCVRegister(idx, #name, RegisterClass::k##class, isPreserved, isAllocatable))).first->second;
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS
    default:
        abort();
    }
}

std::unordered_set<PhysicalRegister *> mir::riscvCallImplicitDsts() {
    std::unordered_set<PhysicalRegister *> implicitDsts;
    for (PhysicalRegister *physReg : riscvRegs()) {
        if (!physReg->isPreserved()) {
            implicitDsts.insert(physReg);
        }
    }
    return implicitDsts;
}

std::unordered_set<PhysicalRegister *> mir::riscvCallImplicitSrcs(int numIntegerArgs, int numFloatingArgs) {
    std::unordered_set<PhysicalRegister *> implicitSrcs{ra(), sp(), gp(), tp(), fp()};
    for (PhysicalRegister *physReg : std::views::take(riscvIntegerArgRegs(), numIntegerArgs)) {
        implicitSrcs.insert(physReg);
    }
    for (PhysicalRegister *physReg : std::views::take(riscvFloatingArgRegs(), numFloatingArgs)) {
        implicitSrcs.insert(physReg);
    }
    return implicitSrcs;
}
