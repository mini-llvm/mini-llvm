// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>
#include <format>
#include <string>

#include "mini-llvm/mir/Constant.h"
#include "mini-llvm/mir/ConstantVisitor.h"

namespace mini_llvm::mir {

class I64Constant final : public Constant {
public:
    explicit I64Constant(int64_t value) : value_(value) {}

    int64_t value() const {
        return value_;
    }

    void setValue(int64_t value) {
        value_ = value;
    }

    int size() const override {
        return 4;
    }

    std::string format() const override {
        return std::format("i64 {}", value());
    }

    void accept(ConstantVisitor &visitor) override {
        visitor.visitI64Constant(*this);
    }

    void accept(ConstantVisitor &visitor) const override {
        visitor.visitI64Constant(*this);
    }

private:
    int64_t value_;
};

} // namespace mini_llvm::mir
