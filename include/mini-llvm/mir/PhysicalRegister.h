#pragma once

#include <string>

#include "mini-llvm/mir/Register.h"
#include "mini-llvm/mir/RegisterClass.h"

namespace mini_llvm::mir {

class PhysicalRegister : public Register {
public:
    virtual int idx() const = 0;
    virtual std::string name() const = 0;
    virtual RegisterClass Class() const = 0;
    virtual bool isPreserved() const = 0;
    virtual bool isAllocatable() const = 0;
    std::string format() const override ;
};

} // namespace mini_llvm::mir
