// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Instruction/Call.h"

#include <memory>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Function.h"
#include "mini-llvm/ir/Instruction.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

Call::Call(std::weak_ptr<Function> callee, std::vector<std::shared_ptr<Value>> args)
        : callee_(this, std::move(callee)) {
    for (auto &arg : args) {
        args_.push_back(std::make_unique<Use<Value>>(this, std::move(arg)));
    }
}

void Call::addArg(const_arg_iterator pos, std::shared_ptr<Value> arg) {
    args_.insert(pos.base(), std::make_unique<Use<Value>>(this, std::move(arg)));
}

void Call::removeArg(const_arg_iterator pos) {
    args_.erase(pos.base());
}

void Call::clearArgs() {
    args_.clear();
}

std::unordered_set<const UseBase *> Call::operands() const {
    std::unordered_set<const UseBase *> operands;
    operands.insert(&callee());
    for (const UseBase &arg : args(*this)) {
        operands.insert(&arg);
    }
    return operands;
}

bool Call::isWellFormed() const {
    if (!Instruction::isWellFormed()) {
        return false;
    }
    std::unique_ptr<FunctionType> functionType = callee()->functionType();
    if (arg_size() < functionType->param_type_size()) {
        return false;
    }
    if (!functionType->isVarArgs() && arg_size() > functionType->param_type_size()) {
        return false;
    }
    for (auto [arg, paramType] : std::views::zip(args(*this), paramTypes(*functionType))) {
        if (*arg->type() != paramType) {
            return false;
        }
    }
    return true;
}

std::string Call::format() const {
    StringJoiner formattedArgs(", ");
    for (const Use<Value> &arg : args(*this)) {
        formattedArgs.add("{} {:o}", *arg->type(), *arg);
    }
    StringJoiner formatted(" ");
    if (*type() != Void()) {
        formatted.add("{:o} =", *this);
    }
    formatted.add("call");
    formatted.add("{}", *callee()->functionType());
    formatted.add("{:o}({})", *callee(), formattedArgs);
    return formatted.toString();
}

std::unique_ptr<Value> Call::clone() const {
    std::vector<std::shared_ptr<Value>> clonedArgs;
    for (const Use<Value> &arg : args(*this)) {
        clonedArgs.push_back(share(*arg));
    }
    return std::make_unique<Call>(share(*callee()), std::move(clonedArgs));
}
