#include "mini-llvm/ir/Value.h"

#include <cassert>
#include <memory>

#include "mini-llvm/ir/Use.h"

using namespace mini_llvm::ir;

bool mini_llvm::ir::replaceAllUsesWith(const Value &value, std::shared_ptr<Value> value2) {
    assert(*value2->type() == *value.type());

    bool changed = false;

    while (!value.use_empty()) {
        value.use_begin()->set(value2);
        changed = true;
    }

    return changed;
}

bool mini_llvm::ir::replaceAllUsesWith(const Value &value, std::weak_ptr<Value> value2) {
    assert(!value2.expired() && *value2.lock()->type() == *value.type());

    bool changed = false;

    while (!value.use_empty()) {
        value.use_begin()->set(value2);
        changed = true;
    }

    return changed;
}
