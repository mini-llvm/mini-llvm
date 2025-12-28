// SPDX-License-Identifier: MIT

#pragma once

#include <string>

#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/targets/riscv/mc/RISCVRegister.h"

namespace mini_llvm::mc {

class RISCVRegisterOperand final : public Operand {
public:
    explicit RISCVRegisterOperand(int idx) : idx_(idx) {}

    int idx() const {
        return idx_;
    }

    void setIdx(int idx) {
        idx_ = idx;
    }

    std::string format() const override {
        return name(static_cast<RISCVRegister>(idx()));
    }

private:
    int idx_;
};

} // namespace mini_llvm::mc
