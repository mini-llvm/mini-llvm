// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <utility>

#include "mini-llvm/mir/Immediate.h"

namespace mini_llvm::mir {

class ImmediateOperand {
public:
    explicit ImmediateOperand(std::unique_ptr<Immediate> imm) : imm_(std::move(imm)) {}

    const Immediate &operator*() const {
        return *imm_;
    }

    const Immediate *operator->() const {
        return &*imm_;
    }

    void set(std::unique_ptr<Immediate> imm) {
        imm_ = std::move(imm);
    }

private:
    std::unique_ptr<Immediate> imm_;
};

} // namespace mini_llvm::mir
