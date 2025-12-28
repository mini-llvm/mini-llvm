// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Store final : public Instruction {
public:
    Store(std::shared_ptr<Value> value, std::shared_ptr<Value> addr)
        : value_(this, std::move(value)), ptr_(this, std::move(addr)) {}

    template <typename Self>
    auto &value(this Self &&self) {
        return self.value_;
    }

    template <typename Self>
    auto &ptr(this Self &&self) {
        return self.ptr_;
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&value(), &ptr()};
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitStore(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitStore(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return std::make_unique<Void>();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    Use<Value> value_;
    Use<Value> ptr_;
};

} // namespace mini_llvm::ir
