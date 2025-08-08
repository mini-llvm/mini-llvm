// SPDX-License-Identifier: MIT

#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/mir/BasicBlockOperand.h"
#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Instruction/Terminator.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT CmpBr : public Terminator {
public:
    CmpBr(int width,
          Condition cond,
          std::shared_ptr<Register> src1,
          std::shared_ptr<Register> src2,
          BasicBlock *trueDest,
          BasicBlock *falseDest)
        : width_(width),
          cond_(cond),
          src1_(RegisterClass::kGPR, std::move(src1)),
          src2_(RegisterClass::kGPR, std::move(src2)),
          trueDest_(trueDest),
          falseDest_(falseDest) {}

    int width() const {
        return width_;
    }

    Condition cond() const {
        return cond_;
    }

    void setCond(Condition cond) {
        cond_ = cond;
    }

    template <typename Self>
    auto &src1(this Self &&self) {
        return self.src1_;
    }

    template <typename Self>
    auto &src2(this Self &&self) {
        return self.src2_;
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
        return {&src1(), &src2()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {&src1(), &src2()};
    }

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    std::string format() const override {
        return std::format(
            "CMPBR i{} {} {}, {}, {:o}, {:o}",
            width() * 8, specifier(cond()), *src1(), *src2(), *trueDest(), *falseDest());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<CmpBr>(
            width(), cond(), share(*src1()), share(*src2()), &*trueDest(), &*falseDest());
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitCmpBr(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitCmpBr(*this);
    }

private:
    int width_;
    Condition cond_;
    RegisterOperand src1_, src2_;
    BasicBlockOperand trueDest_, falseDest_;
};

} // namespace mini_llvm::mir
