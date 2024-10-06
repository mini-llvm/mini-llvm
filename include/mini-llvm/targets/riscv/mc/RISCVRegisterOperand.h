#pragma once

#include <string>

#include "mini-llvm/mc/RegisterOperand.h"
#include "mini-llvm/targets/riscv/mc/RISCVRegister.h"

namespace mini_llvm::mc {

class RISCVRegisterOperand final : public RegisterOperand {
public:
    explicit RISCVRegisterOperand(int idx) : RegisterOperand(idx) {}

protected:
    std::string name(int idx) const override {
        return mc::name(static_cast<RISCVRegister>(idx));
    }
};

} // namespace mini_llvm::mc
