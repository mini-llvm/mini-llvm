// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Terminator.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT CondBr : public Terminator {
public:
    CondBr(Condition cond, BasicBlock *trueDest, BasicBlock *falseDest)
        : cond_(cond), trueDest_(trueDest), falseDest_(falseDest) {}

    Condition cond() const {
        return cond_;
    }

    void setCond(Condition cond) {
        cond_ = cond;
    }

    template <typename Self>
    auto &trueDest(this Self &&self) {
        return self.trueDest_;
    }

    template <typename Self>
    auto &falseDest(this Self &&self) {
        return self.falseDest_;
    }

    std::unordered_set<const BasicBlockOperand *> blockOps() const override {
        return {&trueDest(), &falseDest()};
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
        return std::format("CONDBR {} {:o}, {:o}", specifier(cond()), *trueDest(), *falseDest());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<CondBr>(cond(), &*trueDest(), &*falseDest());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitCondBr(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitCondBr(*this);
    }

private:
    Condition cond_;
    BasicBlockOperand trueDest_, falseDest_;
};

} // namespace mini_llvm::mir
