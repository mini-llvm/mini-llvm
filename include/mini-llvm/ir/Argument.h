#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Strings.h"

namespace mini_llvm::ir {

class Argument final : public Value {
public:
    explicit Argument(std::unique_ptr<Type> type) : type_(std::move(type)) {}

    std::unique_ptr<Type> type() const override {
        return type_->clone();
    }

    std::string format() const override {
        abort();
    }

    std::string formatAsOperand() const override {
        return "%" + (!name().empty() ? name() : "_" + toString(reinterpret_cast<uintptr_t>(this), 62));
    }

    std::unique_ptr<Value> clone() const override {
        abort();
    }

private:
    std::unique_ptr<Type> type_;
};

} // namespace mini_llvm::ir
