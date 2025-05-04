#pragma once

#include <string>
#include <utility>

#include "mini-llvm/mir/Register.h"

namespace mini_llvm::mir {

class VirtualRegister final : public Register {
public:
    const std::string &name() const {
        return name_;
    }

    void setName(std::string name) {
        name_ = std::move(name);
    }

    std::string format() const override;

private:
    std::string name_;
};

} // namespace mini_llvm::mir
