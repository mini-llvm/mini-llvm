// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/InstructionVisitor.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT Call final : public Instruction {
    using ArgumentList = std::vector<std::unique_ptr<Use<Value>>>;

public:
    using arg_iterator = IndirectIterator<ArgumentList::iterator, Use<Value>>;
    using const_arg_iterator = IndirectIterator<ArgumentList::const_iterator, const Use<Value>>;

    explicit Call(std::weak_ptr<Function> callee, std::vector<std::shared_ptr<Value>> args = {});

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

    void accept(InstructionVisitor &visitor) override {
        visitor.visitCall(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitCall(*this);
    }

    bool isWellFormed() const override;

    std::unique_ptr<Type> type() const override {
        return callee()->functionType()->returnType();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    Use<Function> callee_;
    ArgumentList args_;
};

inline std::unique_ptr<FunctionType> functionType(const Call &I) {
    return I.callee()->functionType();
}

inline auto args(Call &I) {
    return std::ranges::subrange(I.arg_begin(), I.arg_end());
}

inline auto args(const Call &I) {
    return std::ranges::subrange(I.arg_begin(), I.arg_end());
}

} // namespace mini_llvm::ir
