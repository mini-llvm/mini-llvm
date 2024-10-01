#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"

namespace mini_llvm::mir {

class RegisterOperand {
public:
    RegisterOperand(RegisterKind kind, std::shared_ptr<Register> reg)
        : kind_(kind), reg_(std::move(reg)) {}

    RegisterKind kind() const {
        return kind_;
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
    RegisterKind kind_;
    std::shared_ptr<Register> reg_;
};

} // namespace mini_llvm::mir
