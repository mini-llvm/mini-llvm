// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <cstdlib>
#include <string>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mc {

class MINI_LLVM_EXPORT RISCVDataDirective final : public Directive {
public:
    RISCVDataDirective(int width, int64_t value)
        : width_(width), value_(value) {}

    int width() const {
        return width_;
    }

    void setWidth(int width) {
        width_ = width;
    }

    int64_t value() const {
        return value_;
    }

    void setValue(int64_t value) {
        value_ = value;
    }

    std::string format() const override;

private:
    int width_;
    int64_t value_;
};

} // namespace mini_llvm::mc
