// SPDX-License-Identifier: MIT

#include <cstdint>
#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/ops/SExt.h"
#include "mini-llvm/common/ops/Trunc.h"
#include "mini-llvm/common/ops/ZExt.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/I16Constant.h"
#include "mini-llvm/ir/Constant/I1Constant.h"
#include "mini-llvm/ir/Constant/I32Constant.h"
#include "mini-llvm/ir/Constant/I64Constant.h"
#include "mini-llvm/ir/Constant/I8Constant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/IntegerCastingOperator.h"
#include "mini-llvm/ir/Instruction/SExt.h"
#include "mini-llvm/ir/Instruction/Trunc.h"
#include "mini-llvm/ir/Instruction/ZExt.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/I1.h"
#include "mini-llvm/ir/Type/I16.h"
#include "mini-llvm/ir/Type/I32.h"
#include "mini-llvm/ir/Type/I64.h"
#include "mini-llvm/ir/Type/I8.h"
#include "mini-llvm/ir/Type/Ptr.h"
#include "mini-llvm/ir/TypeVisitor.h"
#include "mini-llvm/utils/Memory.h"

using namespace mini_llvm;
using namespace mini_llvm::ir;

namespace {

template <typename Op, typename ToConst, typename ToTy>
class ConstantVisitorImpl final : public ConstantVisitor {
public:
    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
    }

    void visitI1Constant(const I1Constant &value) override {
        visit(value);
    }

    void visitI8Constant(const I8Constant &value) override {
        visit(value);
    }

    void visitI16Constant(const I16Constant &value) override {
        visit(value);
    }

    void visitI32Constant(const I32Constant &value) override {
        visit(value);
    }

    void visitI64Constant(const I64Constant &value) override {
        visit(value);
    }

private:
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename FromConst>
    void visit(const FromConst &value) {
        result_.emplace(std::make_shared<ToConst>(Op()(value.value())));
    }
};

template <template <typename To> typename Op>
class TypeVisitorImpl final : public TypeVisitor {
public:
    explicit TypeVisitorImpl(const Constant &value) : value_(value) {}

    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
    }

    void visitI1(const I1 &) override {
        visit<bool, I1Constant, I1>();
    }

    void visitI8(const I8 &) override {
        visit<int8_t, I8Constant, I8>();
    }

    void visitI16(const I16 &) override {
        visit<int16_t, I16Constant, I16>();
    }

    void visitI32(const I32 &) override {
        visit<int32_t, I32Constant, I32>();
    }

    void visitI64(const I64 &) override {
        visit<int64_t, I64Constant, I64>();
    }

private:
    const Constant &value_;
    std::optional<std::shared_ptr<Constant>> result_;

    template <typename To, typename ToConst, typename ToTy>
    void visit() {
        ConstantVisitorImpl<Op<To>, ToConst, ToTy> visitor;
        value_.accept(visitor);
        result_.emplace(visitor.takeResult());
    }
};

template <template <typename To> typename Op>
std::shared_ptr<Constant> foldImpl(const IntegerCastingOperator &I) {
    const Constant &value = static_cast<const Constant &>(*I.value());
    if (dynamic_cast<const PoisonValue *>(&value)) {
        return std::make_shared<PoisonValue>(I.type());
    }
    TypeVisitorImpl<Op> visitor(value);
    I.type()->accept(visitor);
    return visitor.takeResult();
}

std::string formatImpl(const IntegerCastingOperator &I, const char *mnemonic) {
    return std::format("{:o} = {} {} {:o} to {}", I, mnemonic, *I.value()->type(), *I.value(), *I.type());
}

template <typename T>
std::unique_ptr<Value> cloneImpl(const IntegerCastingOperator &I) {
    return std::make_unique<T>(share(*I.value()), cast<IntegerType>(I.type()));
}

} // namespace

std::shared_ptr<Constant> Trunc::fold() const {
    return foldImpl<ops::Trunc>(*this);
}

std::shared_ptr<Constant> SExt::fold() const {
    return foldImpl<ops::SExt>(*this);
}

std::shared_ptr<Constant> ZExt::fold() const {
    return foldImpl<ops::ZExt>(*this);
}

std::string Trunc::format() const {
    return formatImpl(*this, "trunc");
}

std::string SExt::format() const {
    return formatImpl(*this, "sext");
}

std::string ZExt::format() const {
    return formatImpl(*this, "zext");
}

std::unique_ptr<Value> Trunc::clone() const {
    return cloneImpl<Trunc>(*this);
}

std::unique_ptr<Value> SExt::clone() const {
    return cloneImpl<SExt>(*this);
}

std::unique_ptr<Value> ZExt::clone() const {
    return cloneImpl<ZExt>(*this);
}

bool Trunc::isWellFormed() const {
    if (!IntegerCastingOperator::isWellFormed()) {
        return false;
    }
    if (type()->bitSize() >= value()->type()->bitSize()) {
        return false;
    }
    return true;
}

bool SExt::isWellFormed() const {
    if (!IntegerCastingOperator::isWellFormed()) {
        return false;
    }
    if (type()->bitSize() <= value()->type()->bitSize()) {
        return false;
    }
    return true;
}

bool ZExt::isWellFormed() const {
    if (!IntegerCastingOperator::isWellFormed()) {
        return false;
    }
    if (type()->bitSize() <= value()->type()->bitSize()) {
        return false;
    }
    return true;
}
