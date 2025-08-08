// SPDX-License-Identifier: MIT

#include <format>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "mini-llvm/common/ops/FPExt.h"
#include "mini-llvm/common/ops/FPTrunc.h"
#include "mini-llvm/ir/Constant.h"
#include "mini-llvm/ir/Constant/DoubleConstant.h"
#include "mini-llvm/ir/Constant/FloatConstant.h"
#include "mini-llvm/ir/Constant/PoisonValue.h"
#include "mini-llvm/ir/ConstantVisitor.h"
#include "mini-llvm/ir/Instruction/FloatingCastingOperator.h"
#include "mini-llvm/ir/Instruction/FPExt.h"
#include "mini-llvm/ir/Instruction/FPTrunc.h"
#include "mini-llvm/ir/Type.h"
#include "mini-llvm/ir/Type/Double.h"
#include "mini-llvm/ir/Type/Float.h"
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

    void visitFloatConstant(const FloatConstant &value) override {
        visit(value);
    }

    void visitDoubleConstant(const DoubleConstant &value) override {
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
class TypeVisitorImpl : public TypeVisitor {
public:
    explicit TypeVisitorImpl(const Constant &value) : value_(value) {}

    std::shared_ptr<Constant> takeResult() {
        return *std::move(result_);
    }

    void visitFloat(const Float &) override {
        visit<float, FloatConstant, Float>();
    }

    void visitDouble(const Double &) override {
        visit<double, DoubleConstant, Double>();
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
std::shared_ptr<Constant> foldImpl(const FloatingCastingOperator &I) {
    const Constant &value = static_cast<const Constant &>(*I.value());
    if (dynamic_cast<const PoisonValue *>(&value)) {
        return std::make_shared<PoisonValue>(I.type());
    }
    TypeVisitorImpl<Op> visitor(value);
    I.type()->accept(visitor);
    return visitor.takeResult();
}

std::string formatImpl(const FloatingCastingOperator &I, const char *mnemonic) {
    return std::format("{:o} = {} {} {:o} to {}", I, mnemonic, *I.value()->type(), *I.value(), *I.type());
}

template <typename T>
std::unique_ptr<Value> cloneImpl(const FloatingCastingOperator &I) {
    return std::make_unique<T>(share(*I.value()), cast<FloatingType>(I.type()));
}

} // namespace

std::shared_ptr<Constant> FPTrunc::fold() const {
    return foldImpl<ops::FPTrunc>(*this);
}

std::shared_ptr<Constant> FPExt::fold() const {
    return foldImpl<ops::FPExt>(*this);
}

std::string FPTrunc::format() const {
    return formatImpl(*this, "fptrunc");
}

std::string FPExt::format() const {
    return formatImpl(*this, "fpext");
}

std::unique_ptr<Value> FPTrunc::clone() const {
    return cloneImpl<FPTrunc>(*this);
}

std::unique_ptr<Value> FPExt::clone() const {
    return cloneImpl<FPExt>(*this);
}

bool FPTrunc::isWellFormed() const {
    if (!FloatingCastingOperator::isWellFormed()) {
        return false;
    }
    if (type()->bitSize() >= value()->type()->bitSize()) {
        return false;
    }
    return true;
}

bool FPExt::isWellFormed() const {
    if (!FloatingCastingOperator::isWellFormed()) {
        return false;
    }
    if (type()->bitSize() <= value()->type()->bitSize()) {
        return false;
    }
    return true;
}
