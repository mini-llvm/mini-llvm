// SPDX-License-Identifier: MIT

#include "mini-llvm/mc/GlobalValue.h"

#include <memory>
#include <string>

#include "mini-llvm/mc/Directive.h"
#include "mini-llvm/mc/Instruction.h"
#include "mini-llvm/mc/Label.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::mc;

Statement &GlobalValue::add(const_iterator pos, std::unique_ptr<Statement> stmt) {
    return **stmts_.insert(pos.base(), std::move(stmt));
}

void GlobalValue::remove(const_iterator pos) {
    stmts_.erase(pos.base());
}

void GlobalValue::clear() {
    stmts_.clear();
}

std::string GlobalValue::format() const {
    StringJoiner formatted("\n");
    if (isGlobal()) {
        formatted.add("  .globl {}", symbol());
    }
    if (alignment()) {
        formatted.add("  .balign {}", alignment());
    }
    formatted.add("{}:", symbol());
    for (const Statement &stmt : *this) {
        if (dynamic_cast<const Instruction *>(&stmt)) {
            formatted.add("  {}", stmt);
        }
        if (dynamic_cast<const Directive *>(&stmt)) {
            formatted.add("  {}", stmt);
        }
        if (dynamic_cast<const Label *>(&stmt)) {
            formatted.add("{}", stmt);
        }
    }
    return formatted.toString();
}
