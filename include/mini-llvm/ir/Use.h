#pragma once

#include <cassert>
#include <concepts>
#include <memory>
#include <variant>

#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Value.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class UseBase {
public:
    virtual ~UseBase();
    UseBase(Value *user, std::shared_ptr<Value> value);
    UseBase(Value *user, std::weak_ptr<Value> value);
    UseBase(const UseBase &) = delete;
    UseBase(UseBase &&) = delete;
    UseBase &operator=(const UseBase &) = delete;
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

template <typename ValueT = Value, typename TypeT = Type>
    requires std::derived_from<ValueT, Value> && std::derived_from<TypeT, Type>
class Use final : public UseBase {
public:
    Use(Value *user, std::shared_ptr<ValueT> value)
        : UseBase(user,
                  (assert(dynamic_cast<ValueT *>(&*value) && dynamic_cast<TypeT *>(&*value->type())),
                      cast<Value>(std::move(value)))) {}

    Use(Value *user, std::weak_ptr<ValueT> value)
        : UseBase(user,
                  (assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock()) && dynamic_cast<TypeT *>(&*value.lock()->type())),
                      cast<Value>(std::move(value)))) {}

    ValueT &operator*() const override {
        return static_cast<ValueT &>(UseBase::operator*());
    }

    ValueT *operator->() const override {
        return static_cast<ValueT *>(UseBase::operator->());
    }

    void set(std::shared_ptr<Value> value) override {
        assert(dynamic_cast<ValueT *>(&*value) && dynamic_cast<TypeT *>(&*value->type()));
        UseBase::set(std::move(value));
    }

    void set(std::weak_ptr<Value> value) override {
        assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock()) && dynamic_cast<TypeT *>(&*value.lock()->type()));
        UseBase::set(std::move(value));
    }
};

template <typename ValueT, typename TypeT>
    requires std::derived_from<ValueT, Value> && std::derived_from<TypeT, Type> && (!std::same_as<ValueT, Value>)
class Use<ValueT, TypeT> final : public UseBase {
public:
    Use(Value *user, std::shared_ptr<ValueT> value)
        : UseBase(user,
                  (assert(dynamic_cast<ValueT *>(&*value) && dynamic_cast<TypeT *>(&*value->type())),
                      cast<Value>(std::move(value)))) {}

    Use(Value *user, std::weak_ptr<ValueT> value)
        : UseBase(user,
                  (assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock()) && dynamic_cast<TypeT *>(&*value.lock()->type())),
                      cast<Value>(std::move(value)))) {}

    ValueT &operator*() const override {
        return static_cast<ValueT &>(UseBase::operator*());
    }

    ValueT *operator->() const override {
        return static_cast<ValueT *>(UseBase::operator->());
    }

    void set(std::shared_ptr<Value> value) override {
        assert(dynamic_cast<ValueT *>(&*value) && dynamic_cast<TypeT *>(&*value->type()));
        UseBase::set(std::move(value));
    }

    void set(std::weak_ptr<Value> value) override {
        assert(!value.expired() && dynamic_cast<ValueT *>(&*value.lock()) && dynamic_cast<TypeT *>(&*value.lock()->type()));
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
