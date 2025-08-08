// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <format>
#include <string>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/ConstantVisitor.h"

namespace mini_llvm::mir {

class I16Constant final : public Constant {
public:
    explicit I16Constant(int16_t value) : value_(value) {}

    int16_t value() const {
        return value_;
    }

    void setValue(int16_t value) {
        value_ = value;
    }

    int size() const override {
        return 2;
    }

    std::string format() const override {
        return std::format("i16 {}", value());
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitI16Constant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitI16Constant(*this);
    }

private:
    int16_t value_;
};

} // namespace mini_llvm::mir
