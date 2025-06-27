#include "mini-llvm/ir/Function.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Argument.h"
#include "mini-llvm/ir/Attribute.h"
#include "mini-llvm/ir/BasicBlock.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/FunctionType.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

Function::Function(std::unique_ptr<FunctionType> functionType, Linkage linkage)
        : functionType_(std::move(functionType)), linkage_(linkage) {
    for (Type &paramType : paramTypes(*functionType_)) {
        args_.push_back(std::make_shared<Argument>(paramType.clone()));
    }
}

bool Function::hasAttr(Attribute attr) const {
    return std::ranges::contains(attrs_, attr);
}

void Function::setAttr(Attribute attr) {
    if (!std::ranges::contains(attrs_, attr)) {
        attrs_.push_back(attr);
    }
}

void Function::clearAttr(Attribute attr) {
    std::erase(attrs_, attr);
}

BasicBlock &Function::add(Function::const_iterator pos, std::shared_ptr<BasicBlock> block) {
    assert(block->parent_ == nullptr);
    assert(block->parentIterator_ == std::nullopt);
    iterator newPos(blocks_.insert(pos.base(), std::move(block)));
    newPos->parent_ = this;
    newPos->parentIterator_ = newPos;
    return *newPos;
}

BasicBlock &Function::add(Function::const_iterator pos) {
    return add(pos, std::make_shared<BasicBlock>());
}

BasicBlock &Function::prepend(std::shared_ptr<BasicBlock> block) {
    return add(begin(), std::move(block));
}

BasicBlock &Function::prepend() {
    return prepend(std::make_shared<BasicBlock>());
}

BasicBlock &Function::append(std::shared_ptr<BasicBlock> block) {
    return add(end(), std::move(block));
}

BasicBlock &Function::append() {
    return append(std::make_shared<BasicBlock>());
}

void Function::remove(Function::const_iterator pos) {
    pos->parent_ = nullptr;
    pos->parentIterator_ = std::nullopt;
    blocks_.erase(pos.base());
}

void Function::removeFirst() {
    remove(begin());
}

void Function::removeLast() {
    remove(std::prev(end()));
}

void Function::clear() {
    blocks_.clear();
}

std::string Function::format() const {
    StringJoiner formatted(" ");
    if (!isDeclaration()) {
        formatted.add("define");
    } else {
        formatted.add("declare");
    }
    switch (linkage()) {
    case Linkage::kInternal:
        formatted.add("internal");
        break;
    case Linkage::kPrivate:
        formatted.add("private");
        break;
    case Linkage::kExternal:
        break;
    }
    StringJoiner formattedArgs(", ", "(", ")");
    for (const Argument &arg : args(*this)) {
        if (isDeclaration()) {
            formattedArgs.add("{}", *arg.type());
        } else {
            formattedArgs.add("{} {:o}", *arg.type(), arg);
        }
    }
    if (functionType()->isVarArgs()) {
        formattedArgs.add("...");
    }
    formatted.add("{} {:o}{}", *functionType()->returnType(), *this, formattedArgs);
    for (Attribute attr : attrs(*this)) {
        formatted.add(specifier(attr));
    }
    if (!isDeclaration()) {
        StringJoiner formattedBody("\n\n", "{\n", "\n}");
        for (const BasicBlock &B : *this) {
            formattedBody.add("{}", B);
        }
        formatted.add("{}", formattedBody);
    }
    return formatted.toString();
}
