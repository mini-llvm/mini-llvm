#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/mir/Formatting.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/InstructionVisitor.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class FCmp : public Instruction {
public:
    FCmp(Precision precision, std::shared_ptr<Register> src1, std::shared_ptr<Register> src2)
        : precision_(precision),
          src1_(RegisterKind::kFloating, std::move(src1)),
          src2_(RegisterKind::kFloating, std::move(src2)) {}

    Precision precision() const {
        return precision_;
    }

    template <typename Self>
    auto &src1(this Self &&self) {
        return self.src1_;
    }

    template <typename Self>
    auto &src2(this Self &&self) {
        return self.src2_;
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

    bool hasSideEffects() const override {
        return true;
    }

    std::string format() const override {
        return std::format("FCMP<{}> {}, {}", specifier(precision()), *src1(), *src2());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FCmp>(precision(), share(*src1()), share(*src2()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFCmp(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFCmp(*this);
    }

private:
    Precision precision_;
    RegisterOperand src1_, src2_;
};

} // namespace mini_llvm::mir
