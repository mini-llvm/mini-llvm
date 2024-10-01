#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/Function.h"
#include "mini-llvm/mir/FunctionOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/targets/riscv/mir/RISCVInstructionVisitor.h"

namespace mini_llvm::mir {

class RISCVCall final : public Instruction {
public:
    RISCVCall(Function *callee, int numIntegerArgs, int numFloatingArgs)
        : callee_(callee), numIntegerArgs_(numIntegerArgs), numFloatingArgs_(numFloatingArgs) {}

    template <typename Self>
    auto &callee(this Self &&self) {
        return self.callee_;
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
        return {};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {};
    }

    std::unordered_set<PhysicalRegister *> implicitDsts() const override;
    std::unordered_set<PhysicalRegister *> implicitSrcs() const override;

    bool hasSideEffects() const override {
        return true;
    }

    void accept(InstructionVisitor &visitor) override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVCall(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        static_cast<RISCVInstructionVisitor &>(visitor).visitRISCVCall(*this);
    }

    std::string format() const override {
        return std::format("CALL {:o}, {}, {}", *callee(), numIntegerArgs(), numFloatingArgs());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<RISCVCall>(&*callee(), numIntegerArgs(), numFloatingArgs());
    }

private:
    FunctionOperand callee_;
    int numIntegerArgs_, numFloatingArgs_;
};

} // namespace mini_llvm::mir
