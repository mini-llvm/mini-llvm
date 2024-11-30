#include "mini-llvm/ir/Instruction/IndirectCall.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/ir/Type/Void.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

IndirectCall::IndirectCall(std::unique_ptr<FunctionType> functionType, std::shared_ptr<Value> callee, std::vector<std::shared_ptr<Value>> args)
        : functionType_(std::move(functionType)), callee_(this, std::move(callee)) {
    for (auto &arg : args) {
        args_.push_back(std::make_unique<Use<Value>>(this, std::move(arg)));
    }
}

IndirectCall::~IndirectCall() = default;

std::unordered_set<const UseBase *> IndirectCall::operands() const {
    std::unordered_set<const UseBase *> operands;
    operands.insert(&callee());
    for (const UseBase &arg : args(*this)) {
        operands.insert(&arg);
    }
    return operands;
}

std::string IndirectCall::format() const {
    StringJoiner formattedArgs(", ");
    for (const Use<Value> &arg : args(*this)) {
        formattedArgs.addFormat("{} {:o}", *arg->type(), *arg);
    }
    StringJoiner formatted(" ");
    if (*type() != Void()) {
        formatted.addFormat("{:o} =", *this);
    }
    formatted.add("call");
    formatted.addFormat("{}", *type());
    formatted.addFormat("{:o}({})", *callee(), formattedArgs);
    return formatted.toString();
}

std::unique_ptr<Value> IndirectCall::clone() const {
    std::vector<std::shared_ptr<Value>> clonedArgs;
    for (const Use<Value> &arg : args(*this)) {
        clonedArgs.push_back(share(*arg));
    }
    return std::make_unique<IndirectCall>(functionType(), share(*callee()), std::move(clonedArgs));
}
