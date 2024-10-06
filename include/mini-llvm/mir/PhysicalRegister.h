#pragma once

#include <string>

#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterKind.h"

namespace mini_llvm::mir {

class PhysicalRegister : public Register {
public:
    virtual int idx() const = 0;
    virtual std::string name() const = 0;
    virtual RegisterKind kind() const = 0;
    virtual bool isVolatile() const = 0;
    virtual bool isAllocatable() const = 0;

    std::string format() const override {
        return "%" + name();
    }
};

} // namespace mini_llvm::mir
