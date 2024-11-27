#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/RegisterClass.h"

namespace mini_llvm::mir {

class RISCVRegister final : public PhysicalRegister {
public:
    int idx() const override {
        return idx_;
    }

    std::string name() const override {
        return name_;
    }

    RegisterClass Class() const override {
        return class_;
    }

    bool isPreserved() const override {
        return isPreserved_;
    }

    bool isAllocatable() const override {
        return isAllocatable_;
    }

    static RISCVRegister *get(int idx);

private:
    int idx_;
    const char *name_;
    RegisterClass class_;
    bool isPreserved_;
    bool isAllocatable_;

    RISCVRegister(int idx, const char *name, RegisterClass Class, bool isPreserved, bool isAllocatable)
        : idx_(idx), name_(name), class_(Class), isPreserved_(isPreserved), isAllocatable_(isAllocatable) {}
};

namespace riscv {

#define REGS
#define X(idx, name, class, isPreserved, isAllocatable) inline RISCVRegister *name() { return RISCVRegister::get(idx); }
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS

} // namespace riscv

inline std::unordered_set<RISCVRegister *> riscvRegs() {
    using namespace riscv;

    return {
#define REGS
#define X(idx, name, class, isPreserved, isAllocatable) name(),
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS
    };
}

inline std::vector<RISCVRegister *> riscvIntegerResultRegs() {
    using namespace riscv;

    return {
#define INTEGER_RESULT_REGS
#define X(name) name(),
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef INTEGER_RESULT_REGS
    };
}

inline std::vector<RISCVRegister *> riscvIntegerArgRegs() {
    using namespace riscv;

    return {
#define INTEGER_ARG_REGS
#define X(name) name(),
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef INTEGER_ARG_REGS
    };
}

inline std::vector<RISCVRegister *> riscvFloatingResultRegs() {
    using namespace riscv;

    return {
#define FLOATING_RESULT_REGS
#define X(name) name(),
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef FLOATING_RESULT_REGS
    };
}

inline std::vector<RISCVRegister *> riscvFloatingArgRegs() {
    using namespace riscv;

    return {
#define FLOATING_ARG_REGS
#define X(name) name(),
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef FLOATING_ARG_REGS
    };
}

} // namespace mini_llvm::mir
