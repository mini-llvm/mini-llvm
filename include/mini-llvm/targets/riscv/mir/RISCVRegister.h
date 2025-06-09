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
#define X(idx, name, class, isPreserved, isAllocatable) RISCVRegister *name();
#include "mini-llvm/targets/riscv/target.def"
#undef X
#undef REGS

} // namespace riscv

const std::unordered_set<RISCVRegister *> &riscvRegs();
const std::vector<RISCVRegister *> &riscvIntegerResultRegs();
const std::vector<RISCVRegister *> &riscvIntegerArgRegs();
const std::vector<RISCVRegister *> &riscvFloatingResultRegs();
const std::vector<RISCVRegister *> &riscvFloatingArgRegs();
std::unordered_set<PhysicalRegister *> riscvCallImplicitDsts();
std::unordered_set<PhysicalRegister *> riscvCallImplicitSrcs(int numIntegerArgs, int numFloatingArgs);

} // namespace mini_llvm::mir
