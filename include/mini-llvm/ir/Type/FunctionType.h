#pragma once

#include <cstddef>
#include <iterator>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/TypeVisitor.h"

namespace mini_llvm::ir {

class FunctionType final : public Type {
public:
    class param_type_iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Type;
        using difference_type = ptrdiff_t;

    public:
        param_type_iterator() = default;
        Type &operator*() const { return **base_; }
        Type &operator[](difference_type n) const { return *base_[n]; }
        param_type_iterator &operator++() { ++base_; return *this; }
        param_type_iterator &operator--() { --base_; return *this; }
        param_type_iterator operator++(int) { return param_type_iterator(base_++); }
        param_type_iterator operator--(int) { return param_type_iterator(base_--); }
        param_type_iterator &operator+=(difference_type n) { base_ += n; return *this; }
        param_type_iterator &operator-=(difference_type n) { base_ -= n; return *this; }

        friend bool operator==(const param_type_iterator &lhs, const param_type_iterator &rhs) {
            return lhs.base_ == rhs.base_;
        }

        friend bool operator!=(const param_type_iterator &lhs, const param_type_iterator &rhs) {
            return lhs.base_ != rhs.base_;
        }

        friend bool operator<(const param_type_iterator &lhs, const param_type_iterator &rhs) {
            return lhs.base_ < rhs.base_;
        }

        friend bool operator>(const param_type_iterator &lhs, const param_type_iterator &rhs) {
            return lhs.base_ > rhs.base_;
        }

        friend bool operator<=(const param_type_iterator &lhs, const param_type_iterator &rhs) {
            return lhs.base_ <= rhs.base_;
        }

        friend bool operator>=(const param_type_iterator &lhs, const param_type_iterator &rhs) {
            return lhs.base_ >= rhs.base_;
        }

        friend difference_type operator-(const param_type_iterator &lhs, const param_type_iterator &rhs) {
            return lhs.base_ - rhs.base_;
        }

        friend param_type_iterator operator+(param_type_iterator i, difference_type n) { return i += n; }
        friend param_type_iterator operator-(param_type_iterator i, difference_type n) { return i -= n; }

    private:
        std::vector<std::unique_ptr<Type>>::const_iterator base_;

        explicit param_type_iterator(std::vector<std::unique_ptr<Type>>::const_iterator base) : base_(base) {}

        friend class FunctionType;
    };

    FunctionType(std::unique_ptr<Type> returnType, std::vector<std::unique_ptr<Type>> paramTypes, bool isVarArgs)
        : returnType_(std::move(returnType)), paramTypes_(std::move(paramTypes)), isVarArgs_(isVarArgs) {}

    std::unique_ptr<Type> returnType() const {
        return returnType_->clone();
    }

    param_type_iterator param_type_begin() const {
        return param_type_iterator(paramTypes_.begin());
    }

    param_type_iterator param_type_end() const {
        return param_type_iterator(paramTypes_.end());
    }

    bool param_type_empty() const {
        return paramTypes_.empty();
    }

    size_t param_type_size() const {
        return paramTypes_.size();
    }

    bool isVarArgs() const {
        return isVarArgs_;
    }

    int size() const override {
        std::unreachable();
    }

    int alignment() const override {
        std::unreachable();
    }

    std::unique_ptr<Constant> zeroValue() const override {
        std::unreachable();
    }

    std::string format() const override;
    std::unique_ptr<Type> clone() const override;

    void accept(TypeVisitor &visitor) override {
        visitor.visitFunctionType(*this);
    }

    void accept(TypeVisitor &visitor) const override {
        visitor.visitFunctionType(*this);
    }

protected:
    bool equals(const Type &other) const override;

private:
    std::unique_ptr<Type> returnType_;
    std::vector<std::unique_ptr<Type>> paramTypes_;
    bool isVarArgs_;
};

inline auto paramTypes(const FunctionType &type) {
    return std::ranges::subrange(type.param_type_begin(), type.param_type_end());
}

} // namespace mini_llvm::ir
