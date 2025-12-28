// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT IndirectCall : public Instruction {
    using ArgumentList = std::vector<std::unique_ptr<Use<Value>>>;

public:
    using arg_iterator = IndirectIterator<ArgumentList::iterator, Use<Value>>;
    using const_arg_iterator = IndirectIterator<ArgumentList::const_iterator, const Use<Value>>;

    IndirectCall(std::unique_ptr<FunctionType> functionType, std::shared_ptr<Value> callee, std::vector<std::shared_ptr<Value>> args = {});

    std::unique_ptr<FunctionType> functionType() const {
        return cast<FunctionType>(functionType_->clone());
    }

    void setFunctionType(std::unique_ptr<FunctionType> functionType) {
        functionType_ = std::move(functionType);
    }

    template <typename Self>
    auto &callee(this Self &&self) {
        return self.callee_;
    }

    arg_iterator arg_begin() {
        return arg_iterator(args_.begin());
    }

    arg_iterator arg_end() {
        return arg_iterator(args_.end());
    }

    const_arg_iterator arg_begin() const {
        return const_arg_iterator(args_.begin());
    }

    const_arg_iterator arg_end() const {
        return const_arg_iterator(args_.end());
    }

    bool arg_empty() const {
        return args_.empty();
    }

    size_t arg_size() const {
        return args_.size();
    }

    Use<Value> &arg(size_t i) {
        return *args_[i];
    }

    const Use<Value> &arg(size_t i) const {
        return *args_[i];
    }

    void addArg(const_arg_iterator pos, std::shared_ptr<Value> arg);

    void appendArg(std::shared_ptr<Value> arg) {
        addArg(arg_end(), std::move(arg));
    }

    void removeArg(const_arg_iterator pos);

    void removeLastArg() {
        removeArg(std::prev(arg_end()));
    }

    void clearArgs();

    std::unordered_set<const UseBase *> operands() const override;

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return functionType_->returnType();
    }

    void accept(InstructionVisitor &visitor) override {
        visitor.visitIndirectCall(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitIndirectCall(*this);
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    std::unique_ptr<FunctionType> functionType_;
    Use<Value> callee_;
    ArgumentList args_;
};

inline std::unique_ptr<FunctionType> functionType(const IndirectCall &I) {
    return I.functionType();
}

inline auto args(IndirectCall &I) {
    return std::ranges::subrange(I.arg_begin(), I.arg_end());
}

inline auto args(const IndirectCall &I) {
    return std::ranges::subrange(I.arg_begin(), I.arg_end());
}

} // namespace mini_llvm::ir
