#include "mini-llvm/ir/GlobalVar.h"

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/Linkage.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

GlobalVar::GlobalVar(std::unique_ptr<Type> valueType,
                     bool isConstant,
                     Linkage linkage,
                     std::optional<std::shared_ptr<Constant>> initializer)
        : valueType_(std::move(valueType)),
          isConstant_(isConstant),
          linkage_(linkage) {
    if (initializer) {
        assert(*(*initializer_)->type() == *valueType_);
        initializer_.emplace(this, std::move(*initializer));
    }
}

void GlobalVar::setInitializer(std::optional<std::shared_ptr<Constant>> initializer) {
    if (initializer) {
        if (initializer_) {
            initializer_->set(std::move(*initializer));
        } else {
            initializer_.emplace(this, std::move(*initializer));
        }
    } else {
        initializer_.reset();
    }
}

std::string GlobalVar::format() const {
    StringJoiner formatted(" ");
    formatted.addFormat("{} =", formatAsOperand());
    if (linkage() == Linkage::kInternal) {
        formatted.add("internal");
    } else if (linkage() == Linkage::kPrivate) {
        formatted.add("private");
    }
    if (isDeclaration()) {
        formatted.add("external");
    }
    if (isConstant()) {
        formatted.add("constant");
    } else {
        formatted.add("global");
    }
    formatted.addFormat("{}", *valueType());
    if (!isDeclaration()) {
        formatted.addFormat("{:o}", initializer());
    }
    return formatted.toString();
}
