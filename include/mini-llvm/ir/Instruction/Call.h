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
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/IndirectIterator.h"

namespace mini_llvm::ir {

class Call final : public Instruction {
    using ArgumentList = std::vector<std::unique_ptr<Use<Value>>>;

public:
    using arg_iterator = IndirectIterator<ArgumentList::iterator, Use<Value>>;
    using const_arg_iterator = IndirectIterator<ArgumentList::const_iterator, const Use<Value>>;

    explicit Call(std::weak_ptr<Function> callee, std::vector<std::shared_ptr<Value>> args = {});

    ~Call() override;

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

    void addArg(const_arg_iterator pos, std::unique_ptr<Value> arg) {
        args_.insert(pos.base(), std::make_unique<Use<Value>>(this, std::move(arg)));
    }

    void appendArg(std::unique_ptr<Value> arg) {
        addArg(arg_end(), std::move(arg));
    }

    void removeArg(const_arg_iterator pos) {
        args_.erase(pos.base());
    }

    void removeLastArg() {
        removeArg(std::prev(arg_end()));
    }

    void clearArgs() {
        args_.clear();
    }

    std::unordered_set<const UseBase *> operands() const override;

    void accept(InstructionVisitor &visitor) override {
        visitor.visitCall(*this);
    }

    void accept(InstructionVisitor &visitor) const override {
        visitor.visitCall(*this);
    }

    std::unique_ptr<Type> type() const override {
        return callee()->functionType()->returnType();
    }

    std::string format() const override;
    std::unique_ptr<Value> clone() const override;

private:
    Use<Function> callee_;
    ArgumentList args_;
};

inline auto args(Call &I) {
    return std::ranges::subrange(I.arg_begin(), I.arg_end());
}

inline auto args(const Call &I) {
    return std::ranges::subrange(I.arg_begin(), I.arg_end());
}

} // namespace mini_llvm::ir
