// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Constant/VoidValue.h"
#include "mini-llvm/ir/Instruction/Terminator.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Ret final : public Terminator {
public:
    explicit Ret(std::shared_ptr<Value> value = std::make_shared<VoidValue>()) : value_(this, std::move(value)) {}

    template <typename Self>
    auto &value(this Self &&self) {
        return self.value_;
    }

    std::unordered_set<BasicBlock *> successors() const override {
        return {};
    }

    std::unordered_set<const UseBase *> operands() const override {
        return {&value()};
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitRet(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitRet(*this);
    }

    std::unique_ptr<Type> type() const override {
        return std::make_unique<Void>();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    Use<Value> value_;
};

} // namespace mini_llvm::ir
