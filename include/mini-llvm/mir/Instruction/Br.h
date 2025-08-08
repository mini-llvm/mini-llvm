// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction/Terminator.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT Br : public Terminator {
public:
    explicit Br(BasicBlock *dest) : dest_(dest) {}

    template <typename Self>
    auto &dest(this Self &&self) {
        return self.dest_;
    }

    std::unordered_set<const BasicBlockOperand *> blockOps() const override {
        return {&dest()};
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

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    std::string format() const override {
        return std::format("BR {:o}", *dest());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<Br>(&*dest());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitBr(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitBr(*this);
    }

private:
    BasicBlockOperand dest_;
};

} // namespace mini_llvm::mir
