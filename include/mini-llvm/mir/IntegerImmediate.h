// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <memory>

#include "mini-llvm/mir/Immediate.h"

namespace mini_llvm::mir {

class IntegerImmediate final : public Immediate {
public:
    explicit IntegerImmediate(int64_t value) : value_(value) {}

    int64_t value() const override {
        return value_;
    }

    std::unique_ptr<Immediate> clone() const override {
        return std::make_unique<IntegerImmediate>(value());
    }

private:
    int64_t value_;
};

} // namespace mini_llvm::mir
