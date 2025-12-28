// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/common/ExtensionMode.h"
#include "mini-llvm/mir/ImmediateOperand.h"
#include "mini-llvm/mir/Instruction.h"
#include "mini-llvm/mir/MemoryOperand.h"
#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/mir/RegisterOperand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT UnaryOperator : public Instruction {
public:
    int width() const {
        return width_;
    }

    template <typename Self>
    auto &dst(this Self &&self) {
        return self.dst_;
    }

    template <typename Self>
    auto &src(this Self &&self) {
        return self.src_;
    }

    ExtensionMode extMode() const {
        return extMode_;
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

    std::unordered_set<const ImmediateOperand *> immOps() const override {
        return {};
    }

    std::unordered_set<const MemoryOperand *> memOps() const override {
        return {};
    }

    std::string format() const override;

protected:
    UnaryOperator(int width,
                  std::shared_ptr<Register> dst,
                  std::shared_ptr<Register> src,
                  ExtensionMode extMode)
        : width_(width),
          dst_(RegisterClass::kGPR, std::move(dst)),
          src_(RegisterClass::kGPR, std::move(src)),
          extMode_(extMode) {}

    virtual const char *mnemonic() const = 0;

private:
    int width_;
    RegisterOperand dst_, src_;
    ExtensionMode extMode_;
};

} // namespace mini_llvm::mir
