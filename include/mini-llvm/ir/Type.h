#pragma once

#include <concepts>
#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/TypeVisitor.h"

namespace mini_llvm::ir {

class Constant;

class Type {
public:
    virtual ~Type() = default;
    Type() = default;
    Type(const Type &) = delete;
    Type(Type &&) = delete;
    Type &operator=(const Type &) = delete;
    Type &operator=(Type &&) = delete;
    virtual int size() const = 0;
    virtual int alignment() const = 0;
    virtual int size(int) const { return size(); }
    virtual int alignment(int) const { return alignment(); }
    virtual std::unique_ptr<Constant> zeroValue() const = 0;
    virtual std::string format() const = 0;
    virtual std::unique_ptr<Type> clone() const = 0;
    virtual void accept(TypeVisitor &visitor) = 0;
    virtual void accept(TypeVisitor &visitor) const = 0;

protected:
    virtual bool equals(const Type &other) const = 0;

    friend bool operator==(const Type &lhs, const Type &rhs);
};

inline bool operator==(const Type &lhs, const Type &rhs) {
    return lhs.equals(rhs);
}

} // namespace mini_llvm::ir

template <typename TypeT>
    requires std::derived_from<TypeT, mini_llvm::ir::Type>
struct std::formatter<TypeT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    auto format(const TypeT &type, std::format_context &ctx) const {
        return std::format_to(ctx.out(), "{}", type.format());
    }
};
