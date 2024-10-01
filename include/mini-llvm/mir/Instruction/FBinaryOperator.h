#pragma once

#include <format>
#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/common/Precision.h"
#include "mini-llvm/mir/Formatting.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"
#include "mini-llvm/mir/RegisterOperand.h"

namespace mini_llvm::mir {

class FBinaryOperator : public Instruction {
public:
    Precision precision() const {
        return precision_;
    }

    template <typename Self>
    auto &dst(this Self &&self) {
        return self.dst_;
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
        return {&dst(), &src1(), &src2()};
    }

    std::unordered_set<const RegisterOperand *> dsts() const override {
        return {&dst()};
    }

    std::unordered_set<const RegisterOperand *> srcs() const override {
        return {&src1(), &src2()};
    }

    std::string format() const override {
        return std::format("{}<{}> {}, {}, {}", mnemonic(), specifier(precision()), *dst(), *src1(), *src2());
    }

protected:
    FBinaryOperator(Precision precision,
                    std::shared_ptr<Register> dst,
                    std::shared_ptr<Register> src1,
                    std::shared_ptr<Register> src2)
        : precision_(precision),
          dst_(RegisterKind::kFloating, std::move(dst)),
          src1_(RegisterKind::kFloating, std::move(src1)),
          src2_(RegisterKind::kFloating, std::move(src2)) {}

    virtual const char *mnemonic() const = 0;

private:
    Precision precision_;
    RegisterOperand dst_, src1_, src2_;
};

} // namespace mini_llvm::mir
