#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Terminator.h"
#include "mini-llvm/targets/riscv/mir/RISCVInstructionVisitor.h"

namespace mini_llvm::mir {

class RISCVRet final : public Terminator {
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

    std::unordered_set<const BasicBlockOperand *> successors() const override {
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

    void accept(InstructionVisitor &visitor) override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVRet(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVRet(*this);
    }

    std::string format() const override {
        return std::format("RET {}, {}", numIntegerResults(), numFloatingResults());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<RISCVRet>(numIntegerResults(), numFloatingResults());
    }

private:
    int numIntegerResults_, numFloatingResults_;
};

} // namespace mini_llvm::mir
