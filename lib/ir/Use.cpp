// SPDX-License-Identifier: MIT

#include "mini-llvm/ir/Use.h"

#include <cassert>
#include <memory>
#include <utility>
#include <variant>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/GlobalValue.h"
#include "mini-llvm/ir/Value.h"

using namespace mini_llvm::ir;

UseBase::UseBase(Value *user, std::shared_ptr<Value> value) : user_(user) {
    if (dynamic_cast<Constant *>(&*value) && !dynamic_cast<GlobalValue *>(&*value)) {
        value_ = std::move(value);
    } else {
        value_ = std::weak_ptr<Value>(value);
    }
    (*this)->uses_.insert(this);
}

UseBase::UseBase(Value *user, std::weak_ptr<Value> value) : user_(user) {
    assert(!value.expired() && !(dynamic_cast<Constant *>(&*value.lock()) && !dynamic_cast<GlobalValue *>(&*value.lock())));
    value_ = std::move(value);
    (*this)->uses_.insert(this);
}

UseBase::~UseBase() {
    if (auto *value = std::get_if<std::weak_ptr<Value>>(&value_); value && value->expired()) {
        return;
    }
    (*this)->uses_.erase(this);
}

Value &UseBase::operator*() const {
    if (auto *value = std::get_if<std::weak_ptr<Value>>(&value_)) {
        return *value->lock();
    } else {
        return *std::get<std::shared_ptr<Value>>(value_);
    }
}

void UseBase::set(std::shared_ptr<Value> value) {
    (*this)->uses_.erase(this);
    if (dynamic_cast<Constant *>(&*value) && !dynamic_cast<GlobalValue *>(&*value)) {
        value_ = std::move(value);
    } else {
        value_ = std::weak_ptr<Value>(value);
    }
    (*this)->uses_.insert(this);
}

void UseBase::set(std::weak_ptr<Value> value) {
    assert(!value.expired() && !(dynamic_cast<Constant *>(&*value.lock()) && !dynamic_cast<GlobalValue *>(&*value.lock())));
    (*this)->uses_.erase(this);
    value_ = std::move(value);
    (*this)->uses_.insert(this);
}

bool UseBase::expired() const {
    if (auto *value = std::get_if<std::weak_ptr<Value>>(&value_)) {
        return value->expired();
    } else {
        return false;
    }
}
