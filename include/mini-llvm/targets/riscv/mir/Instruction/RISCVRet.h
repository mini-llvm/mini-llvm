// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Terminator.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/targets/riscv/mir/RISCVInstructionVisitor.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT RISCVRet final : public Terminator {
public:
    RISCVRet(int numIntegerResults, int numFloatingResults)
        : numIntegerResults_(numIntegerResults), numFloatingResults_(numFloatingResults) {}

    int numIntegerResults() const {
        return numIntegerResults_;
    }

    void setNumIntegerResults(int numIntegerResults) {
        numIntegerResults_ = numIntegerResults;
    }

    int numFloatingResults() const {
        return numFloatingResults_;
    }

    void setNumFloatingResults(int numFloatingResults) {
        numFloatingResults_ = numFloatingResults;
    }

    std::unordered_set<const BasicBlockOperand *> blockOps() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> regOps() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {};
    }

    std::unordered_set<PhysicalRegister *> implicitDsts() const override {
        return {};
    }

    std::unordered_set<PhysicalRegister *> implicitSrcs() const override;

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    void accept(InstructionVisitor &visitor) override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVRet(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVRet(*this);
    }

    std::string format() const override;

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<RISCVRet>(numIntegerResults(), numFloatingResults());
    }

private:
    int numIntegerResults_, numFloatingResults_;
};

} // namespace mini_llvm::mir
