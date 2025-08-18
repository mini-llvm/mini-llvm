// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <cstddef>
#include <format>
#include <memory>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>

#include "mini-llvm/ir/Type.h"
#include "mini-llvm/utils/Compiler.h"
#include "mini-llvm/utils/IndirectIterator.h"
#include "mini-llvm/utils/Memory.h"

namespace mini_llvm::ir {

class UseBase;

class MINI_LLVM_EXPORT Value : public std::enable_shared_from_this<Value> {
    using UseSet = std::unordered_set<UseBase *>;

public:
    using use_iterator = IndirectIterator<UseSet::iterator, UseBase>;

    virtual ~Value() = default;

    Value() = default;

    Value(const Value &) = delete;
    Value &operator=(const Value &) = delete;

    Value(Value &&) = delete;
    Value &operator=(Value &&) = delete;

    const std::string &name() const & {
        return name_;
    }

    std::string &&name() && {
        return std::move(name_);
    }

    void setName(std::string name) {
        name_ = std::move(name);
    }

    std::string formatName() const;

    use_iterator use_begin() const {
        return use_iterator(uses_.begin());
    }

    use_iterator use_end() const {
        return use_iterator(uses_.end());
    }

    bool use_empty() const {
        return uses_.empty();
    }

    size_t use_size() const {
        return uses_.size();
    }

    virtual bool isWellFormed() const {
        return true;
    }

    virtual std::unique_ptr<Type> type() const = 0;

    virtual std::string format() const = 0;
    virtual std::string formatAsOperand() const = 0;

    virtual std::unique_ptr<Value> clone() const = 0;

private:
    std::string name_;
    mutable UseSet uses_;

    friend class UseBase;
};

inline auto uses(const Value &value) {
    return std::ranges::subrange(value.use_begin(), value.use_end());
}

MINI_LLVM_EXPORT bool replaceAllUsesWith(const Value &value, std::shared_ptr<Value> newValue);
MINI_LLVM_EXPORT bool replaceAllUsesWith(const Value &value, std::weak_ptr<Value> newValue);

template <typename ValueT>
    requires std::derived_from<ValueT, Value>
bool replaceAllUsesWith(const Value &value, std::shared_ptr<ValueT> newValue) {
    return replaceAllUsesWith(value, cast<Value>(std::move(newValue)));
}

template <typename ValueT>
    requires std::derived_from<ValueT, Value>
bool replaceAllUsesWith(const Value &value, std::weak_ptr<ValueT> newValue) {
    return replaceAllUsesWith(value, cast<Value>(std::move(newValue)));
}

} // namespace mini_llvm::ir

template <typename ValueT>
    requires std::derived_from<ValueT, mini_llvm::ir::Value>
struct std::formatter<ValueT> {
    constexpr auto parse(std::format_parse_context &ctx) {
        if (*ctx.begin() == 'o') {
            asOperand_ = true;
            return std::next(ctx.begin());
        }
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const ValueT &value, FormatContext &ctx) const {
        if (asOperand_) {
            return std::format_to(ctx.out(), "{}", value.formatAsOperand());
        }
        return std::format_to(ctx.out(), "{}", value.format());
    }

private:
    bool asOperand_ = false;
};
