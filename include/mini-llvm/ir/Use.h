// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <concepts>
#include <memory>
#include <utility>
#include <variant>

#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class MINI_LLVM_EXPORT UseBase {
public:
    virtual ~UseBase();

    UseBase(Value *user, std::shared_ptr<Value> value);
    UseBase(Value *user, std::weak_ptr<Value> value);

    UseBase(const UseBase &) = delete;
    UseBase &operator=(const UseBase &) = delete;

    UseBase(UseBase &&) = delete;
    UseBase &operator=(UseBase &&) = delete;

    Value *user() const { return user_; }
    virtual Value &operator*() const;
    virtual Value *operator->() const { return &operator*(); }
    virtual void set(std::shared_ptr<Value> value);
    virtual void set(std::weak_ptr<Value> value);
    bool expired() const;

private:
    Value *user_;
    std::variant<std::monostate, std::shared_ptr<Value>, std::weak_ptr<Value>> value_;
};

template <typename ValueT = Value>
    requires std::derived_from<ValueT, Value>
class Use final : public UseBase {
public:
    Use(Value *user, std::shared_ptr<ValueT> value)
        : UseBase(user, (assert(dynamic_cast<ValueT *>(&*value)), cast<Value>(std::move(value)))) {}

    Use(Value *user, std::weak_ptr<ValueT> value)
        : UseBase(user, (assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock())), cast<Value>(std::move(value)))) {}

    ValueT &operator*() const override {
        return static_cast<ValueT &>(UseBase::operator*());
    }

    ValueT *operator->() const override {
        return static_cast<ValueT *>(UseBase::operator->());
    }

    void set(std::shared_ptr<Value> value) override {
        assert(dynamic_cast<ValueT *>(&*value));
        UseBase::set(std::move(value));
    }

    void set(std::weak_ptr<Value> value) override {
        assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock()));
        UseBase::set(std::move(value));
    }
};

template <typename ValueT>
    requires (std::derived_from<ValueT, Value> && !std::same_as<ValueT, Value>)
class Use<ValueT> final : public UseBase {
public:
    Use(Value *user, std::shared_ptr<ValueT> value)
        : UseBase(user, (assert(dynamic_cast<ValueT *>(&*value)), cast<Value>(std::move(value)))) {}

    Use(Value *user, std::weak_ptr<ValueT> value)
        : UseBase(user, (assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock())), cast<Value>(std::move(value)))) {}

    ValueT &operator*() const override {
        return static_cast<ValueT &>(UseBase::operator*());
    }

    ValueT *operator->() const override {
        return static_cast<ValueT *>(UseBase::operator->());
    }

    void set(std::shared_ptr<Value> value) override {
        assert(dynamic_cast<ValueT *>(&*value));
        UseBase::set(std::move(value));
    }

    void set(std::weak_ptr<Value> value) override {
        assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock()));
        UseBase::set(std::move(value));
    }

    void set(std::shared_ptr<ValueT> value) {
        UseBase::set(cast<Value>(std::move(value)));
    }

    void set(std::weak_ptr<ValueT> value) {
        UseBase::set(cast<Value>(std::move(value)));
    }
};

} // namespace mini_llvm::ir
