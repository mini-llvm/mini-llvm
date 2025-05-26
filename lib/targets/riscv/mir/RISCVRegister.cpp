#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

#include <cstdlib>
#include <memory>

#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/utils/HashMap.h"

using namespace mini_llvm::mir;

RISCVRegister *RISCVRegister::get(int idx) {
    static HashMap<int, std::shared_ptr<RISCVRegister>> pool;

    if (pool.contains(idx)) {
        return &*pool[idx];
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
