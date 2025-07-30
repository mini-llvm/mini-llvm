#pragma once

#include <cstdint>
#include <string>

#include "mini-llvm/mc/Operand.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT ImmediateOperand final : public Operand {
public:
    explicit ImmediateOperand(int64_t value) : value_(value) {}

    int64_t value() const {
        return value_;
    }

    void setValue(int64_t value) {
        value_ = value;
    }

    std::string format() const override {
        return std::to_string(value());
    }

private:
    int64_t value_;
};

} // namespace mini_llvm::mc
