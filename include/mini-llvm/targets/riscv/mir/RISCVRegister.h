// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT RISCVRegister final : public PhysicalRegister {
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

    int width() const override {
        return width_;
    }

    bool isPreserved() const override {
        return isPreserved_;
    }

    bool isAllocatable() const override {
        return isAllocatable_;
    }

    static RISCVRegister *get(int idx);
    static RISCVRegister *get(std::string_view name);

private:
    int idx_;
    const char *name_;
    RegisterClass class_;
    int width_;
    bool isPreserved_;
    bool isAllocatable_;

    RISCVRegister(int idx, const char *name, RegisterClass Class, int width, bool isPreserved, bool isAllocatable)
        : idx_(idx), name_(name), class_(Class), width_(width), isPreserved_(isPreserved), isAllocatable_(isAllocatable) {}
};

MINI_LLVM_EXPORT const std::unordered_set<RISCVRegister *> &riscvRegs();
MINI_LLVM_EXPORT const std::vector<RISCVRegister *> &riscvIntegerResultRegs();
MINI_LLVM_EXPORT const std::vector<RISCVRegister *> &riscvIntegerArgRegs();
MINI_LLVM_EXPORT const std::vector<RISCVRegister *> &riscvFloatingResultRegs();
MINI_LLVM_EXPORT const std::vector<RISCVRegister *> &riscvFloatingArgRegs();
MINI_LLVM_EXPORT std::unordered_set<PhysicalRegister *> riscvCallImplicitDsts();
MINI_LLVM_EXPORT std::unordered_set<PhysicalRegister *> riscvCallImplicitSrcs(int numIntegerArgs, int numFloatingArgs);

} // namespace mini_llvm::mir
