#include "mini-llvm/targets/riscv/mir/RISCVRegister.h"

#include <memory>
#include <unordered_map>
#include <utility>

#include "mini-llvm/mir/RegisterKind.h"

using namespace mini_llvm::mir;

RISCVRegister *RISCVRegister::get(int idx) {
    static std::unordered_map<int, std::shared_ptr<RISCVRegister>> pool;

    if (pool.contains(idx)) {
        return &*pool[idx];
    }

    switch (idx) {
#define REGS
#define X(idx, name, kind, isVolatile, isAllocatable) \
    case idx: \
        return &*pool.insert( \
            {idx, std::shared_ptr<RISCVRegister>(new RISCVRegister(idx, #name, RegisterKind::k##kind, isVolatile, isAllocatable))}).first->second;
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS
    default:
        std::unreachable();
    }
}
