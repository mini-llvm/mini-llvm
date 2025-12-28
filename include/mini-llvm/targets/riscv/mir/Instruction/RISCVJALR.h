// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/targets/riscv/mir/RISCVInstructionVisitor.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT RISCVJALR final : public Instruction {
public:
    RISCVJALR(std::shared_ptr<Register> src, int numIntegerArgs, int numFloatingArgs)
        : src_(RegisterClass::kGPR, std::move(src)),
          numIntegerArgs_(numIntegerArgs),
          numFloatingArgs_(numFloatingArgs) {}

    template <typename Self>
    auto &src(this Self &&self) {
        return self.src_;
    }

    int numIntegerArgs() const {
        return numIntegerArgs_;
    }

    void setNumIntegerArgs(int numIntegerArgs) {
        numIntegerArgs_ = numIntegerArgs;
    }

    int numFloatingArgs() const {
        return numFloatingArgs_;
    }

    void setNumFloatingArgs(int numFloatingArgs) {
        numFloatingArgs_ = numFloatingArgs;
    }

    std::unordered_set<const RegisterOperand *> regOps() const override {
        return {&src()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {&src()};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    std::unordered_set<PhysicalRegister *> implicitDsts() const override;
    std::unordered_set<PhysicalRegister *> implicitSrcs() const override;

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    bool hasSideEffects() const override {
        return true;
    }

    void accept(InstructionVisitor &visitor) override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVJALR(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVJALR(*this);
    }

    std::string format() const override;

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<RISCVJALR>(share(*src()), numIntegerArgs(), numFloatingArgs());
    }

private:
    RegisterOperand src_;
    int numIntegerArgs_, numFloatingArgs_;
};

} // namespace mini_llvm::mir
