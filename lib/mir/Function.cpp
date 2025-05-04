#include "mini-llvm/mir/Function.h"

#include <algorithm>
#include <cctype>
#include <format>
#include <memory>
#include <string>
#include <utility>

#include "mini-llvm/mir/BasicBlock.h"
#include "mini-llvm/utils/StringJoiner.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm::mir;

BasicBlock &Function::add(const_iterator pos, std::unique_ptr<BasicBlock> block) {
    return **blocks_.insert(pos.base(), std::move(block));
}

std::unique_ptr<BasicBlock> Function::remove(const_iterator pos) {
    std::unique_ptr<BasicBlock> block = std::move(const_cast<std::unique_ptr<BasicBlock> &>(*pos.base()));
    blocks_.erase(pos.base());
    return block;
}

std::string Function::format() const {
    if (empty()) {
        return std::format("undefined {}", formatAsOperand());
    }
    StringJoiner formattedHeader(" ");
    if (linkage() == Linkage::kInternal) {
        formattedHeader.add("internal");
    }
    if (linkage() == Linkage::kPrivate) {
        formattedHeader.add("private");
    }
    formattedHeader.addFormat("{:o}:", *this);
    StringJoiner formattedBody("\n\n");
    for (const BasicBlock &B : *this) {
        formattedBody.addFormat("{}", B);
    }
    return std::format("{}\n{}", formattedHeader, formattedBody);
}

std::string Function::formatAsOperand() const {
    if (!std::ranges::all_of(name(), [](char ch) { return isalnum(ch) || ch == '_' || ch == '.'; })) {
        return "@" + quote(name());
    }
    return "@" + name();
}
