#pragma once

#include <cstdint>
#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Value.h"

namespace mini_llvm::ir {

class Argument final : public Value {
public:
    explicit Argument(std::unique_ptr<Type> type) : type_(std::move(type)) {}

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override {
        std::unreachable();
    }

    std::string formatAsOperand() const override {
        return "%" + (!name().empty() ? name() : std::format("{:x}", reinterpret_cast<uintptr_t>(this)));
    }

    std::unique_ptr<Value> clone() const override {
        std::unreachable();
    }

private:
    std::unique_ptr<Type> type_;
};

} // namespace mini_llvm::ir
