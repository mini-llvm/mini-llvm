// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Constant/ArrayConstant.h"

#include <cassert>
#include <cstdint>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/ArrayType.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/Use.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Ascii.h"
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

bool isZeroInitializer(const ArrayConstant &C) {
    for (const Use<Constant> &element : elements(C)) {
        if (*element != *element->type()->zeroValue()) {
            return false;
        }
    }
    return true;
}

std::string formatAsString(const ArrayConstant &C) {
    std::string formatted;
    formatted += "c\"";
    for (const Use<Constant> &element : elements(C)) {
        int8_t value = static_cast<const I8Constant *>(&*element)->value();
        if (value == '\\') {
            formatted += "\\\\";
        } else if (isAsciiPrintable(value) && value != '"') {
            formatted += value;
        } else {
            formatted += std::format("\\{:02X}", static_cast<uint8_t>(value));
        }
    }
    formatted += "\"";
    return formatted;
}

std::string formatAsArray(const ArrayConstant &C) {
    StringJoiner formatted(", ", "[", "]");
    for (const Use<Constant> &element : elements(C)) {
        formatted.add("{} {:o}", *element->type(), *element);
    }
    return formatted.toString();
}

} // namespace

ArrayConstant::ArrayConstant(std::unique_ptr<ArrayType> type, std::vector<std::shared_ptr<Constant>> elements)
        : type_(std::move(type)) {
#ifndef NDEBUG
    assert(elements.size() <= type_->numElements());
    for (const std::shared_ptr<Constant> &element : elements) {
        assert(*element->type() == *type_->elementType());
    }
#endif
    for (std::shared_ptr<Constant> &element : elements) {
        elements_.push_back(std::make_unique<Use<Constant>>(this, std::move(element)));
    }
    if (elements_.size() < type_->numElements()) {
        std::shared_ptr<Constant> zeroValue = type_->elementType()->zeroValue();
        while (elements_.size() < type_->numElements()) {
            elements_.push_back(std::make_unique<Use<Constant>>(this, zeroValue));
        }
    }
}

bool ArrayConstant::isWellFormed() const {
    if (!Constant::isWellFormed()) {
        return false;
    }
    if (elements_.size() != type_->numElements()) {
        return false;
    }
    std::unique_ptr<Type> elementType = static_cast<const ArrayType *>(&*type())->elementType();
    for (const Use<Constant> &element : elements(*this)) {
        if (*element->type() != *elementType) {
            return false;
        }
    }
    return true;
}

std::string ArrayConstant::format() const {
    if (isZeroInitializer(*this)) {
        return "zeroinitializer";
    }
    if (*static_cast<const ArrayType *>(&*type())->elementType() == I8()) {
        return formatAsString(*this);
    }
    return formatAsArray(*this);
}

std::unique_ptr<Value> ArrayConstant::clone() const {
    std::unique_ptr<ArrayType> clonedType = cast<ArrayType>(type());
    std::vector<std::shared_ptr<Constant>> clonedElements;
    for (const Use<Constant> &element : elements(*this)) {
        clonedElements.push_back(share(*element));
    }
    return std::make_unique<ArrayConstant>(std::move(clonedType), std::move(clonedElements));
}

bool ArrayConstant::equals(const Constant &other) const {
    if (typeid(*this) != typeid(other)) {
        return false;
    }
    const ArrayConstant &castOther = static_cast<const ArrayConstant &>(other);
    if (*type() != *castOther.type()) {
        return false;
    }
    for (auto [element1, element2] : std::views::zip(elements(*this), elements(castOther))) {
        if (*element1 != *element2) {
            return false;
        }
    }
    return true;
}
