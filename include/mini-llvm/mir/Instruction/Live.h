#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/PhysicalRegister.h"
#include "mini-llvm/mir/RegisterOperand.h"

namespace mini_llvm::mir {

class Live : public Instruction {
public:
    explicit Live(PhysicalRegister *physReg) : physReg_(physReg) {}

    PhysicalRegister *physReg() const {
        return physReg_;
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

    std::unordered_set<PhysicalRegister *> implicitSrcs() const override {
        return {physReg()};
    }

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    bool hasSideEffects() const override {
        return true;
    }

    std::string format() const override {
        return std::format("LIVE {}", *physReg());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Live>(physReg());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitLive(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitLive(*this);
    }

private:
    PhysicalRegister *physReg_;
};

} // namespace mini_llvm::mir
