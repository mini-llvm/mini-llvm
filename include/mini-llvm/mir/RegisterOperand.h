// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"

namespace mini_llvm::mir {

class RegisterOperand {
public:
    RegisterOperand(RegisterClass Class, std::shared_ptr<Register> reg)
        : class_(Class), reg_(std::move(reg)) {}

    RegisterClass Class() const {
        return class_;
    }

    Register &operator*() const {
        return *reg_;
    }

    Register *operator->() const {
        return &*reg_;
    }

    void set(std::shared_ptr<Register> reg) {
        reg_ = std::move(reg);
    }

private:
    RegisterClass class_;
    std::shared_ptr<Register> reg_;
};

} // namespace mini_llvm::mir
