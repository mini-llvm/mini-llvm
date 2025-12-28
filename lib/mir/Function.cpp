// SPDX-License-Identifier: MIT

#include "mini-llvm/mir/Function.h"

#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mir;

BasicBlock &Function::add(const_iterator pos, std::unique_ptr<BasicBlock> block) {
    return **blocks_.insert(pos.base(), std::move(block));
}

std::unique_ptr<BasicBlock> Function::remove(const_iterator pos) {
    std::unique_ptr<BasicBlock> block = std::move(const_cast<std::unique_ptr<BasicBlock> &>(*pos.base()));
    blocks_.erase(pos.base());
    return block;
}

void Function::clear() {
    while (!empty()) {
        removeFirst();
    }
}

std::string Function::format() const {
    if (isDeclaration()) {
        return std::format("undefined {}", formatAsOperand());
    }
    StringJoiner formattedHeader(" ");
    switch (linkage()) {
    case Linkage::kInternal:
        formattedHeader.add("internal");
        break;
    case Linkage::kPrivate:
        formattedHeader.add("private");
        break;
    case Linkage::kExternal:
        break;
    }
    formattedHeader.add("{:o}:", *this);
    StringJoiner formattedBody("\n\n");
    for (const BasicBlock &B : *this) {
        formattedBody.add("{}", B);
    }
    return std::format("{}\n{}", formattedHeader, formattedBody);
}
