// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/GlobalVar.h"

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <variant>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

GlobalVar::GlobalVar(std::unique_ptr<Type> valueType,
                     Linkage linkage,
                     bool isConstant,
                     std::shared_ptr<Constant> initializer)
        : valueType_(std::move(valueType)),
          linkage_(linkage),
          isConstant_(isConstant) {
    setInitializer(std::move(initializer));
}

GlobalVar::GlobalVar(std::unique_ptr<Type> valueType, Linkage linkage)
        : valueType_(std::move(valueType)),
          linkage_(linkage),
          isConstant_(false) {
    setInitializer(nullptr);
}

Constant &GlobalVar::initializer() {
    if (auto *initializer = std::get_if<std::weak_ptr<Constant>>(&initializer_)) {
        return *initializer->lock();
    } else {
        return *std::get<std::shared_ptr<Constant>>(initializer_);
    }
}

const Constant &GlobalVar::initializer() const {
    if (auto *initializer = std::get_if<std::weak_ptr<Constant>>(&initializer_)) {
        return *initializer->lock();
    } else {
        return *std::get<std::shared_ptr<Constant>>(initializer_);
    }
}

void GlobalVar::setInitializer(std::shared_ptr<Constant> initializer) {
    if (initializer) {
        assert(*initializer->type() == *valueType_);
        if (dynamic_cast<const GlobalValue *>(&*initializer)) {
            initializer_ = std::weak_ptr<Constant>(initializer);
        } else {
            initializer_ = std::move(initializer);
        }
    } else {
        initializer_ = std::monostate{};
    }
}

bool GlobalVar::isDeclaration() const {
    return std::holds_alternative<std::monostate>(initializer_);
}

std::string GlobalVar::format() const {
    StringJoiner formatted(" ");
    formatted.add("{} =", formatAsOperand());
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
    if (isDeclaration()) {
        formatted.add("external");
    }
    if (isConstant()) {
        formatted.add("constant");
    } else {
        formatted.add("global");
    }
    formatted.add("{}", *valueType());
    if (!isDeclaration()) {
        formatted.add("{:o}", initializer());
    }
    return formatted.toString();
}
