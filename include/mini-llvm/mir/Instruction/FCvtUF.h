#pragma once

#include <format>
#include <memory>
#include <unordered_set>
#include <utility>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/mir/Formatting.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::mir {

class FCvtUF : public Instruction {
public:
    FCvtUF(int dstWidth, Precision srcPrecision, std::shared_ptr<Register> dst, std::shared_ptr<Register> src)
        : dstWidth_(dstWidth),
          srcPrecision_(srcPrecision),
          dst_(RegisterKind::kInteger, std::move(dst)),
          src_(RegisterKind::kFloating, std::move(src)) {}

    int dstWidth() const {
        return dstWidth_;
    }

    Precision srcPrecision() const {
        return srcPrecision_;
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
        return true;
    }

    std::string format() const override {
        return std::format("FCVTUF<{}, {}> {}, {}", dstWidth(), specifier(srcPrecision()), *dst(), *src());
    }

    std::unique_ptr<Instruction> clone() const override {
        return std::make_unique<FCvtUF>(dstWidth(), srcPrecision(), share(*dst()), share(*src()));
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitFCvtUF(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitFCvtUF(*this);
    }

private:
    int dstWidth_;
    Precision srcPrecision_;
    RegisterOperand dst_, src_;
};

} // namespace mini_llvm::mir
