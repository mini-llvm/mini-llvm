// SPDX-License-Identifier: MIT

#pragma once

#include <string>

#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT PhysicalRegister : public Register {
public:
    virtual int idx() const = 0;
    virtual std::string name() const = 0;
    virtual RegisterClass Class() const = 0;
    virtual bool isPreserved() const = 0;
    virtual bool isAllocatable() const = 0;

    std::string format() const override {
        return "$" + name();
    }
};

} // namespace mini_llvm::mir
