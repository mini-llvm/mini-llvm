#pragma once

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/mir/Condition.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class CmpZSet : public Instruction {
public:
    CmpZSet(int dstWidth,
            int srcWidth,
            Condition cond,
            std::shared_ptr<Register> dst,
            std::shared_ptr<Register> src)
        : dstWidth_(dstWidth),
          srcWidth_(srcWidth),
          cond_(cond),
          dst_(RegisterKind::kInteger, std::move(dst)),
          src_(RegisterKind::kInteger, std::move(src)) {}

    int dstWidth() const {
        return dstWidth_;
    }

    int srcWidth() const {
        return srcWidth_;
    }

    Condition cond() const {
        return cond_;
    }

    void setCond(Condition cond) {
        cond_ = cond;
    }

    template <typename Self>
    auto &dst(this Self &&self) {
        return self.dst_;
    }

    template <typename Self>
    auto &src(this Self &&self) {
        return self.src_;
    }

    std::unordered_set<const RegisterOperand *> regOps() const override {
        return {&dst(), &src()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {&dst()};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {&src()};
    }

    bool hasSideEffects() const override {
        return false;
    }

    std::string format() const override {
        return std::format("CMPZSET<{}, {}> {}, {}, {}", dstWidth(), srcWidth(), specifier(cond()), *dst(), *src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<CmpZSet>(
            dstWidth(), srcWidth(), cond(), share(*dst()), share(*src()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitCmpZSet(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitCmpZSet(*this);
    }

private:
    int dstWidth_, srcWidth_;
    Condition cond_;
    RegisterOperand dst_, src_;
};

} // namespace mini_llvm::mir
