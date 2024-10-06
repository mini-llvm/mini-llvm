#include "mini-llvm/ir/Constant/ArrayConstant.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
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
#include "mini-llvm/utils/Memory.h"
#include "mini-llvm/utils/StringJoiner.h"

using namespace mini_llvm::ir;

ArrayConstant::ArrayConstant(std::unique_ptr<ArrayType> type, std::vector<std::shared_ptr<Constant>> elements)
        : type_(std::move(type)) {
    assert(elements.size() <= type_->numElements());
#ifndef NDEBUG
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

std::string ArrayConstant::format() const {
    if (std::ranges::all_of(elements(*this), [](const Use<Constant> &element) {
        return *element == *element->type()->zeroValue();
    })) {
        return "zeroinitializer";
    }
    if (*static_cast<const ArrayType *>(&*type())->elementType() == I8()) {
        StringJoiner formatted("", "c\"", "\"");
        for (const Use<Constant> &element : elements(*this)) {
            int8_t value = static_cast<const I8Constant *>(&*element)->value();
            if (value == static_cast<int8_t>('\\')) {
                formatted.add("\\\\");
            } else if (0x20 <= value && value <= 0x7e) {
                formatted.addFormat("{:c}", value);
            } else {
                formatted.addFormat("\\{:02X}", value);
            }
        }
        return formatted.toString();
    } else {
        StringJoiner formatted(", ", "[", "]");
        for (const Use<Constant> &element : elements(*this)) {
            formatted.addFormat("{} {}", *element->type(), *element);
        }
        return formatted.toString();
    }
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
    if (typeid(*this) != typeid(other)) return false;
    const ArrayConstant &castOther = static_cast<const ArrayConstant &>(other);
    if (*type() != *castOther.type()) return false;
    for (auto [element1, element2] : std::views::zip(elements(*this), elements(castOther))) {
        if (*element1 != *element2) return false;
    }
    return true;
}

std::vector<std::shared_ptr<Constant>> mini_llvm::ir::flatten(const Constant &C) {
    std::vector<std::shared_ptr<Constant>> flattened;
    if (auto *arrayConstant = dynamic_cast<const ArrayConstant *>(&C)) {
        for (const Use<Constant> &element : elements(*arrayConstant)) {
            std::vector<std::shared_ptr<Constant>> flattenedElement = flatten(*element);
            flattened.insert(flattened.end(), flattenedElement.begin(), flattenedElement.end());
        }
    } else {
        flattened.push_back(cast<Constant>(C.clone()));
    }
    return flattened;
}
