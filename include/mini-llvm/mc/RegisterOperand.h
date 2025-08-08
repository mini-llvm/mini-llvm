// SPDX-License-Identifier: MIT

#pragma once

#include <string>

#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT RegisterOperand : public Operand {
public:
    explicit RegisterOperand(int idx) : idx_(idx) {}

    int idx() const {
        return idx_;
    }

    void setIdx(int idx) {
        idx_ = idx;
    }

    std::string format() const override {
        return name(idx());
    }

protected:
    virtual std::string name(int idx) const = 0;

private:
    int idx_;
};

} // namespace mini_llvm::mc
