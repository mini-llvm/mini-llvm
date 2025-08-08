// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Value.h"

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "mini-llvm/ir/Use.h"
#include "mini-llvm/utils/Ascii.h"
#include "mini-llvm/utils/Strings.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

bool shouldQuote(std::string_view str) {
    for (char ch : str) {
        if (!isAsciiLetterOrDigit(ch) && ch != '$' && ch != '.' && ch != '_') {
            return true;
        }
    }
    return false;
}

std::string quote(std::string_view str) {
    std::string quoted;
    quoted += '"';
    for (char ch : str) {
        if (ch == '\\') {
            quoted += "\\\\";
        } else if (isAsciiPrintable(ch) && ch != '"') {
            quoted += ch;
        } else {
            quoted += std::format("\\{:02X}", ch);
        }
    }
    quoted += '"';
    return quoted;
}

} // namespace

std::string ir::Value::formatName() const {
    if (name_.empty()) {
        return "_" + toString(reinterpret_cast<uintptr_t>(this), 62);
    }
    if (shouldQuote(name_)) {
        return quote(name_);
    }
    return name_;
}

bool ir::replaceAllUsesWith(const Value &value, std::shared_ptr<Value> newValue) {
    assert(*newValue->type() == *value.type());

    bool changed = false;

    while (!value.use_empty()) {
        value.use_begin()->set(newValue);
        changed = true;
    }

    return changed;
}

bool ir::replaceAllUsesWith(const Value &value, std::weak_ptr<Value> newValue) {
    assert(!newValue.expired() && *newValue.lock()->type() == *value.type());

    bool changed = false;

    while (!value.use_empty()) {
        value.use_begin()->set(newValue);
        changed = true;
    }

    return changed;
}
