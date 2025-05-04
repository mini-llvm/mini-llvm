#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "mini-llvm/mir/Register.h"
#include "mini-llvm/utils/Strings.h"

namespace mini_llvm::mir {

class VirtualRegister final : public Register {
public:
    const std::string &name() const {
        return name_;
    }

    void setName(std::string name) {
        name_ = std::move(name);
    }

    std::string format() const override {
        return "$" + (!name().empty() ? name() : "_" + toString(reinterpret_cast<uintptr_t>(this), 62));
    }

private:
    std::string name_;
};

} // namespace mini_llvm::mir
