// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <cstdint>
#include <format>
#include <memory>
#include <string>

#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Compiler.h"

namespace mini_llvm::ir {

class Constant;

class MINI_LLVM_EXPORT Type {
public:
    virtual ~Type() = default;

    Type() = default;

    Type(const Type &) = delete;
    Type &operator=(const Type &) = delete;

    Type(Type &&) = delete;
    Type &operator=(Type &&) = delete;

    virtual int size() const = 0;
    virtual int alignment() const = 0;
    virtual int size(int) const { return size(); }
    virtual int alignment(int) const { return alignment(); }

    virtual int bitSize() const {
        return size() * 8;
    }

    virtual int bitAlignment() const {
        return alignment() * 8;
    }

    virtual int bitSize(int pointerSize) const {
        return size(pointerSize) * 8;
    }

    virtual int bitAlignment(int pointerAlignment) const {
        return alignment(pointerAlignment) * 8;
    }

    virtual std::unique_ptr<Constant> zeroValue() const = 0;
    virtual std::unique_ptr<Constant> constant(int64_t value) const = 0;
    virtual std::unique_ptr<Type> promoted() const = 0;
    virtual std::unique_ptr<Type> demoted() const = 0;
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

    template <typename FormatContext>
    auto format(const TypeT &type, FormatContext &ctx) const {
        return std::format_to(ctx.out(), "{}", type.format());
    }
};
