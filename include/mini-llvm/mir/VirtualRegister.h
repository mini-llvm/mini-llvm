// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::mir {

class MINI_LLVM_EXPORT VirtualRegister final : public Register {
public:
    explicit VirtualRegister(int width)
        : width_(width) {}

    const std::string &name() const & {
        return name_;
    }

    std::string &&name() && {
        return std::move(name_);
    }

    void setName(std::string name) {
        name_ = std::move(name);
    }

    int width() const override {
        return width_;
    }

    std::string format() const override;

private:
    std::string name_;
    int width_;
};

} // namespace mini_llvm::mir
